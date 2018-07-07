#include "File.hpp"
#include "Event.hpp"
#include "Signal.hpp"
#include "Desktop.hpp"
#include "Error.hpp"
#include "Pipe.hpp"

namespace
{
	// Look for the fswatch program

	sys::file::path const &GetProgramPath()
	{
		constexpr auto const program = "fswatch";
		static sys::file::path const path = sys::GetProgramPath("fswatch");
		if (path.empty())
		{
			SDL::SetError(CannotFindProgram, program);
		}
		return path;
	}

	// Watch for file changes in another thread

	class FileWatch
	{
	public:

		FileWatch(sys::file::path path)
		{
			static sys::file::path const program = GetProgramPath();
			if (not program.empty())
			{
				sys::file::path const command = fmt::join(program, path, " ");
				thread = std::thread([command] { Thread(command); });
			}
		}

	private:

		std::thread thread;

 		static void Thread(sys::file::path command)
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

		FileNotify &Get(sys::file::path path)
		{
			auto const begin = watchers.lower_bound(path); // not < path
			auto const end = watchers.upper_bound(path); // > path
			// Terminate any redundant watcher processes
			if (watchers.end() != begin)
			{
				watchers.erase(begin, end);
			}
			// Launch a new process
			watchers.emplace(path, path);
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
			try
			{
				std::string const path = SDL::GetUserEventData(event);
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
: Slot(FileMonitor::Instance().Get(path), observer)
{}

