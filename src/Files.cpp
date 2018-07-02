#include "Files.hpp"
#include "System.hpp"
#include "Manager.hpp"
#include "Error.hpp"
#include "Event.hpp"

namespace
{
	// Folder and file name constants

	constexpr char const *Added = "Added";
	constexpr char const *Removed = "Removed";
	constexpr char const *Folder = "FileMonitor";

	// Manager to monitor files and send notifications

	using Watch = Relay<sys::file::Notify*, std::string_view>;

	class FileMonitor : public Manager<Watch*, sys::file::path>
	{
	public:

		static FileMonitor &Instance()
		{
			static FileMonitor singleton;
			return singleton;
		}

	private:

		// File events that are sent through the main queue
		enum FileEventCode { UpdateMonitor, UpdateWatcher };

		// Own directory where monitor reads notifications
		sys::file::path const self = sys::GetTemporaryPath(Folder);

		// Thread control data
		std::future<void> future;

		// The monitoring thread
		void Thread();

		FileManager()
		{
			if (self.empty())
			{
				SDL::LogError(Folder);
				return;
			}
			future = std::async([this]()
			{
				Thread();
			});
		}

		~FileManager()
		{
			if (future.valid())
			{
				// Remove own directory from watched files
				sys::file::path const path = self / Removed;
				sys::file::append(path, self.string());
				future.wait(); // join thread when done
			}
		}

		// The following methods are called in the main thread
		
		void Generate(std::vector<sys::file::path> &ids) const override
		{
			std::ofstream stream(self / Added);
			for (sys::file::path const &path : ids)
			{
				stream << path;
			}
		}

		void Destroy(std::vector<sys::file::path> const &ids) const override
		{
			std::ofstream stream(self / Removed);
			for (sys::file::path const &path : ids)
			{
				stream << path;
			}
		}

		bool SendUpdate() const override
		{
			return SDL::SendUserUpdate(UpdateFiles, UpdateMonitor);
		}

		void Dispatch(std::string_view path) const
		{
			sys::file::path path = SDL::GetUserEventData(event);
			Find(path).Emit(path);
		}

		static void UpdateHandler(SDL_Event const &event)
		{
			switch (event.user.code)
			{
			case UpdateMonitor:
				Instance().Update();
				break;
			case UpdateWatcher:
				Instance().Dispatch(event);
				break;
			default:
				assert(not "FileEventCode");
			}
		}

		unsigned UpdateEvent = SDL::UserEvent(UpdateFiles);
		ScopedEventHandler updater{ UpdateEvent, UpdateHandler };

		// The following method is called in the monitoring thread

		using Callback = std::function<void(std::string_view)>;

		bool NotifyChanged(sys::file::path const &path, Callback&& add, Callback&& remove)
		{
			bool quit = false;
			if (not (self < path))
			{
				// Queue the file event so that it is updated in the main thread
				if (not SDL::SendUserEvent(UpdateFiles, UpdateWatcher, path))
				{
					SDL::LogError(CannotSendEvent);
				}
			}
			else // update self
			{
				if (path.filename() == Added)
				{
					std::string line;
					std::ifstream stream(path);
					while (std::getline(stream, line))
					{
						if (not (self < line))
						{
							add(line);
						}
					}
				}
				else
				if (path.filename() == Removed)
				{
					std::string line;
					std::ifstream stream(path);
					while (std::getline(stream, line))
					{
						if (not (self < line))
						{
							remove(line);
						}
						else
						if (self == line)
						{
							quit = true;
						}
					}
				}
				// Clear file contents
				sys::file::truncate(path);
			}
			return quit;
		}
	};
}

// Slot mechanism

sys::file::Notify::Notify(std::string_view path, Observer observer)
: Slot(FileMonitor::Find(path), observer)
{}

// Platform dependent monitoring thread

#if defined(__LINUX__)

#include <sys/inotify.h>

void FileMonitor::Thread()
{
	int const fd = inotify_init();
	if (-1 == fd)
	{
		SDL::perror("inotify_init");
		return;
	}

	std::map<sys::file::path, int> data;

	auto add = [&](sys::file::path const &path)
	{
		int const wd = inotify_add_watch(fd, path.c_str(), IN_MODIFY);
		if (-1 == wd)
		{
			SDL::perror("inotify_add_watch");
		}
		else
		{
			data[path] = wd;
		}
	};
	
	auto remove = [&](sys::file::path const &path)
	{
		int const wd = data.at(path);
		if (-1 == inotify_rm_watch(fd, wd))
		{
			SDL::perror("inotify_rm_watch");
		}
		data.erase(path);
	};

	add(self);
	bool quit = false;
	while (not quit) try
	{
		char buf[BUFSIZ];
		// Block until there are events to process
		ssize_t const sz = read(fd, buf, sizeof(buf));
		if (-1 == sz)
		{
			if (SDL::perror("inotify read") and SDL::ShowError(SDL_MESSAGEBOX_WARNING))
			{
				continue;
			}
			else
			{
				break;
			}
		}
		else
		if (0 == sz)
		{
			SDL::Log("inotify EOF");
			break;
		}

		// Process all buffered file events
		union
		{
		 inotify_event *ev;
		 char *it;
		};
		for (it = buf; it < buf+sz; it += sizeof(inotify_event) + ev->len)
		{
			std::string_view path(ev->name, ev->len);
			quit |= NotifyChanged(path, add, remove);
		}
	}
	catch (std::exception const &exception)
	{
		SDL::SetError(CaughtException, exception.what());
		// Return from thread if user chooses on exception
		done = not SDL::ShowError(SDL_MESSAGEBOX_WARNING);
	}
}

#elif defined(__BSD__)

#include <sys/event.h>

void FileMonitor::Thread()
{
	int const kq = kqueue();
	if (-1 == kq)
	{
		SDL::perror("kqueue");
		return;
	}

	std::vector<struct kevent> all;
	std::vector<struct kevent> ev;

	while (not done) try
	{
		UpdateWatchedFiles
		(
			[&](sys::file::path const &path) // on add
			{
				int const fd = open(path.c_str(), O_EVTONLY);
				if (-1 == fd)
				{
					SDL::perror("kqueue open");
				}
				else
				{
					struct kevent ev;
					EV_SET(&ev, fd, EVFILT_VNODE, EV_ADD|EV_CLEAR, NOTE_WRITE, 0, path.c_str());
					all.push_back(ev);
				}
			}
			,
			[&](sys::file::path const &path) // on remove
			{
				all.erase(stl::find_if(all, [&](struct kevent &ev))
				{
					union
					{
						void *address;
						char *string;
					};
					address = ev.udata;
					return path == string;
				}));
			}
		);

		// Block until there are events to process
		int const nev = kevent(kq, all.data(), all.size(), ev.data(), ev.size(), nullptr);
		if (-1 == nev)
		{
			if (SDL::perror("kevent") and SDL::ShowError(SDL_MESSAGEBOX_WARNING))
			{
				continue;
			}
			else
			{
				break;
			}
		}

		for (int it = 0; it < nev; ++it)
		{
			if (EV_ERROR == ev[it].flags)
			{
				if (SDL::SetErrno(ev[it].data))
				{
					SDL::perror("EV_ERROR");
				}
			}
			else
			{
				union
				{
					void *address;
					char *string;
				};
				address = kev[ev].udata;
				ProcessChange(string);
			}
		}
	}
	catch (std::exception const &exception)
	{
		QueryError(exception);
	}
}

#elif defined(__WIN32__)

namespace
{
	bool LogLastError(char const *prefix)
	{
		LPSTR buffer = nullptr;
		std::size_t const size = FormatMessage
		( FORMAT_MESSAGE_ALLOCATE_BUFFER
		| FORMAT_MESSAGE_FROM_SYSTEM
		| FORMAT_MESSAGE_IGNORE_INSERTS
		, nullptr
		, GetLastError()
		, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
		, (LPSTR) &buffer
		, 0
		, nullptr
		);

		std::string message(buffer, size);
		LocalFree(buffer);
		return SDL::SetError(message) and SDL::LogError(prefix);
	}
}

void FileMonitor::Thread()
{
	std::map<sys::file::path, HANDLE> data;

	while (not done) try
	{
		// Update our list of directories watched

		UpdateWatchedFiles
		(
			[&](sys::file::path const &path) // on add
			{
				constexpr DWORD filter = FILE_NOTIFY_CHANGE_LAST_WRITE;
				HANDLE const handle = FindFirstChangeNotification(path.c_str(), TRUE, filter);
				if (INVALID_HANDLE_VALUE == handle)
				{
					LogLastError("FindFirstChangeNotification");
				}
				else
				{
					data[path] = handle;
				}
			}
			,
			[&](sys::file::path const &path) // on remove
			{
				HANDLE const handle = data.at(path);
				if (FALSE == FindCloseChangeNotification(handle))
				{
					LogLastError("FindCloseChangeNotification");
				}
				else
				{
					watched.erase(path);
				}
			}
		);

		std::vector<HANDLE> obj(data.size());
		algo::transform(data, obj, [](auto const &pair) { return pair.second; });

		// Block until there are events to process
		DWORD const status = WaitForMultipleObjects(obj.size(), obj.data(), FALSE, INFINITE);
		if (WAIT_FAILED == status)
		{
			LogLastError("WaitForMultipleObjects");
			return;
		}

		HANDLE const handle = obj.at(status - WAIT_OBJECT_0);
		if (FALSE == FindNextChangeNotification(handle))
		{
			LogLastError("FindNextChangeNotification");
			continue;
		}


	}
	catch (std::exception const &exception)
	{
		QueryError(exception);
	}
}

#endif // operating systems
