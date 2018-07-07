#include "File.hpp"
#include "Event.hpp"
#include "Signal.hpp"
#include "Desktop.hpp"
#include "Error.hpp"
#include "Pipe.hpp"
#include "filesystem.hpp"

namespace
{
	// Look for the fswatch program

	sys::file::path const &GetProgramPath()
	{
		constexpr auto const program = "fswatch";
		static sys::file::path const path = sys::GetProgramPath("fswatch");
		if (path.empty())
		{
			SDL::SetError(CannotFindPath, program);
		}
		return fmt::quote(path);
	}

	// Create a command for an fswatch process

	sys::file::path GetShellCommand(sys::file::path path)
	{
		static sys::file::path const &program = GetProgramPath();
		if (not program.empty())
		{
			return fmt::join(program, path, " ");
		}
		return program;
	}

	// Watch for file changes in another thread

	class FileWatch
	{
	public:

		FileWatch(sys::file::path path)
		: command(GetShellCommand(path))
		{
			if (not command.empty())
			{
				SDL::Log(ProcessStarting, command);
				thread = std::thread([this] { Thread(command); });
			}
		}

		~FileWatch()
		{
			if (not command.empty())
			{
				SDL::Log(ProcessTerminating, command);
			}
		}

	private:

		sys::file::path const command;
		std::thread thread;

 		static void Thread(sys::file::path const &command)
		{
			// Start a process with the fswatch program
			sys::io::File process = SDL::Process(command);
			if (not process)
			{
				SDL::LogError(command);
				return;
			}
			// Block for input
			std::string line;
			while (process.get(line))
			{
				// Push onto the main event queue
				if (not SDL::SendUserEvent(UpdateFile, 0, line))
				{
					SDL::LogError(CannotSendEvent);
				}
			}
		}
	};

	// Manager to monitor files and send notifications

	using FileNotify = Signal<sys::file::Notify *, std::string_view>;

	class FileMonitor
	{
	public:

		static FileMonitor &Instance()
		{
			static FileMonitor singleton;
			return singleton;
		}

		FileNotify &Notify(std::string_view string)
		{
			sys::file::path const path = sys::file::canonical(string);
			// Terminate any redundant processes
			auto const begin = watchers.upper_bound(path); // not equal
			if (watchers.end() != begin)
			{
				auto const end = std::find_if_not(begin, watchers.end(),
					[&](sys::file::path const &other)
					{ 
						return path < other;
					}
				);
				watchers.erase(begin, end);
			}
			// Start a new process if required
			watchers.try_emplace(path, path);
			// Return the signal handler
			return notifiers[path];
		}

	private:

		std::map<sys::file::path, FileNotify> notifiers;
		std::map<sys::file::path, FileWatch> watchers;

		// Dispatch file events to observers

		static unsigned const UpdateEvent = SDL::UserEvent(UpdateFile);

		static void UpdateHandler(SDL_Event const &event)
		{
			std::string const path = SDL::GetUserEventData(event);
			try
			{
				Instance().notifiers.at(path).Emit(path);
			}
			catch (std::exception const &except)
			{
				SDL::LogError(error);
			}
		}

		ScopedEventHandler updater{ UpdateEvent, UpdateHandler };
	}
}

// Slot mechanism

sys::file::Notify::Notify(std::string_view path, Observer observer)
: Slot(FileMonitor::Instance().Notify(path), observer)
{}

