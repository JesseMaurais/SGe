#include "Files.hpp"
#include "System.hpp"
#include "Desktop.hpp"
#include "Manager.hpp"
#include "Error.hpp"
#include "std.hpp"

namespace fs = stl::filesystem;

namespace
{
	// Files that provide monitoring commands
	constexpr char const *AddedFilename = "add";
	constexpr char const *RemovedFilename = "remove";
	// Folder monitored for asynchronous commands
	constexpr char const *Folder = "FileWatch";


	class FileMonitor : public ManagedResources
	{
	private:

		// Thread control data
		std::future<void> future;
		bool done = false;

		// Monitored file data
		std::set<fs::path> watched;
		std::vector<fs::path> added;
		std::vector<fs::path> removed;

		fs::path const self;

		FileMonitor()
		: self(sys::GetTemporaryPath(Folder))
		{
			if (self.empty())
			{
				SDL::LogError(Folder);
				return;
			}
			added.push_back(self);
			future = std::async([this]()
			{
				Thread(); 
			});
		}
		~FileMonitor()
		{
			if (not self.empty())
			{
				RemoveWatch({self});
				future.wait();
			}
		}

		void AddWatch(std::vector<fs::path> const &paths)
		{
			std::ofstream stream(self/AddedFilename);
			for (fs::path const &path : paths)
			{
				stream << path;
			}
		}

		void RemoveWatch(std::vector<fs::path> const &paths)
		{
			std::ofstream stream(self/RemovedFilename);
			for (fs::path const &path : paths)
			{
				stream << path;
			}
		}

		void Thread(); // platform dependent

		using Callback = std::function<void(fs::path const &path)>;

		void UpdateWatchedFiles(Callback&& onAdd, Callback&& onRemove)
		{
			// Add before remove so that we can replace paths to watch

			for (fs::path const &path : added)
			{
				// Remove any sub directory of added path
				auto const upper = watched.upper_bound(path);
				if (watched.end() != upper)
				{
					removed.push_back(path);
				}
				// Add only if path not a sub directory
				auto const lower = watched.lower_bound(path);
				if (watched.end() == lower)
				{
					auto pair = watched.insert(path);
					assert(pair.second);
					onAdd(*pair.first);
				}
			}

			for (fs::path const &path : removed)
			{
				auto const it = watched.find(path);
				assert(watched.end() != it);
				if (watched.end() != it)
				{
					watched.erase(it);
					onRemove(path);
				}
			}
		}

		void ProcessChange(fs::path const &path)
		{
			if (self < path) // update self
			{
				std::string string = path.filename();
				// Filename selects the operation
				if (string == AddedFilename)
				{
					std::ifstream stream(path);
					while (std::getline(stream, string))
					{
						if (not (self < string))
						{
							added.push_back(string);
						}
					}
				}
				else
				if (string == RemovedFilename)
				{
					std::ifstream stream(path);
					while (std::getline(stream, string))
					{
						if (not (self < string))
						{
							removed.push_back(string);
						}
						else
						if (self == string)
						{
							done = true;
						}
					}
				}
				// Clear contents
				stl::truncate(path);
			}
			else
			{
				NotifyChange(path);
			}
		}

		void NotifyChange(fs::path const &path)
		{
			(void) path;
		}

		void QueryError(std::exception const &exception)
		{
			SDL::LogError(String(CaughtException), exception.what());
			done = not SDL::ShowError(SDL_MESSAGEBOX_WARNING);
		}

	public:

		static FileMonitor &Instance()
		{
			static FileMonitor singleton;
			return singleton;
		}
	};
}

Resources &FileWatch::Manager()
{
	return FileMonitor::Instance();
}


#if defined(__LINUX__)

#include <sys/inotify.h>
#include <unistd.h>

void FileMonitor::Thread()
{
	int const fd = inotify_init();
	if (-1 == fd)
	{
		SDL::perror("inotify_init");
		return;
	}

	std::map<fs::path, int> data;
	char buf[BUFSIZ];

	while (not done) try
	{
		UpdateWatchedFiles
		(
			[&](fs::path const &path) // on add
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
			}
			,
			[&](fs::path const &path) // on remove
			{
				int const wd = data.at(path);
				if (-1 == inotify_rm_watch(fd, wd))
				{
					SDL::perror("inotify_rm_watch");
				}
				data.erase(path);
			}
		);

		// Block until there are events to process
		ssize_t const sz = read(fd, buf, sizeof(buf));
		if (-1 == sz)
		{
			SDL::perror("inotify read");
			if (SDL::ShowError(SDL_MESSAGEBOX_WARNING))
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
			ProcessChange(fs::path(ev->name, ev->name + ev->len));
		}
	}
	catch (std::exception const &exception)
	{
		QueryError(exception);
	}
}

#else // BSD
#if defined(__BSD__)

#warning "BSD interface is not complete"

#include <sys/event.h>


void FileWatcher::Thread()
{
	int const kq = kqueue();
	if (-1 == kq)
	{
		SDL::perror("kqueue");
		return;
	}

	std::vector<struct kevent> monitor;
	std::vector<struct kevent> events;

	while (not done) try
	{
		UpdateWatchedFiles
		(
			[&](fs::path const &path) // on add
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
					monitor.push_back(ev);
				}
			}
			,
			[&](fs::path const &path) // on remove
			{
				auto it = stl::find_if(monitor, [&](struct kevent &ev))
				{
					union { void *address; char *string; };
					address = ev.udata;
					return path == string;
				});
				assert(monitor.end() != it);
				if (monitor.end() != it)
				{
					EV_SET()
					monitor.erase(it);

				}
			}
		);

		// Block until there are events to process
		int const nev = kevent(kq, monitor.data(), monitor.size(), events.data(), events.size(), nullptr);
		if (-1 == nev)
		{
			SDL::SetError(ColonSeparator, "kevent", std::strerror(errno));
			if (SDL::ShowError(SDL_MESSAGEBOX_WARNING))
			{
				continue;
			}
			else
			{
				break;
			}
		}

		for (int ev = 0; ev < nev; ++ev)
		{
			if (EV_ERROR == events[ev].flags)
			{
				std::uintptr_t error = events[ev].data;
				SDL::perror("EV_ERROR", std::strerror(error));
			}
			else
			{
				union { void *address; char *string; };
				address = events[ev].udata;
				ProcessChange(string);
			}
		}
	}
	catch (std::exception const &exception)
	{
		QueryError(exception);
	}
}

#else // WINDOWS
#if defined(__WINDOWS__)

#warning "Win32 interface is not complete"

namespace
{
	bool SetLastError(std::string const &prefix)
	{
		LPSTR buffer = nullptr;
		size_t size = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), &buffer, 0, nullptr);

		std::string message(buffer, size);
		LocalFree(buffer);
		return SDL::perror(prefix, message);
	}
}

void FileMonitor::Thread()
{
	std::map<fs::path, HANDLE> data;

	while (not done) try
	{
		// Update our list of directories watched

		UpdateWatchedFiles
		(
			[&](fs::path const &path) // on add
			{
				constexpr DWORD filter = FILE_NOTIFY_CHANGE_LAST_WRITE;
				HANDLE const handle = FindFirstChangeNotification(path.c_str(), TRUE, filter);
				if (INVALID_HANDLE_VALUE == handle)
				{
					SetLastError("FindFirstChangeNotification");
				}
				else
				{
					data[path] = handle;
				}
			}
			,
			[&](fs::path const &path) // on remove
			{
				HANDLE const handle = data.at(path);
				if (FALSE == FindCloseChangeNotification(handle))
				{
					SetLastError("FindCloseChangeNotification");
				}
				else
				{
					watched.erase(path);
				}
			}
		);

		std::vector<HANDLE> obj(data.size());
		stl::transform(watched, obj, [](auto const &pair) { return pair.second; });

		// Block until there are events to process
		DWORD const status = WaitForMultipleObjects(obj.size(), obj.data(), FALSE, INFINITE);
		if (WAIT_FAILED == status)
		{
			SetLastError("WaitForMultipleObjects");
			return;
		}
		
		HANDLE const handle = obj.at(status - WAIT_OBJECT_0);
		if (FALSE == FindNextChangeNotification(handle))
		{
			SetLastError("FindNextChangeNotification");
			continue;
		}

	
	}
	catch (std::exception const &exception)
	{
		QueryError(exception);
	}
}

#endif // WINDOWS
#endif // BSD
#endif // LINUX

