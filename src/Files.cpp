#include "Files.hpp"
#include "System.hpp"
#include "Desktop.hpp"
#include "Error.hpp"
#include "std.hpp"

namespace fs = stl::filesystem;

namespace
{
	class FileMonitor : public Resources
	{
	private:

		// Thread control data
		std::future<void> future;
		bool done = false;

		// Monitored file data
		std::set<fs::path> watched;
		std::vector<fs::path> added;
		std::vector<fs::path> removed;

		// File names for monitor commands
		constexpr char const *addedFilename = "add";
		constexpr char const *removedFilename = "remove";
		// Folder monitored for asynchronous commands
		constexpr char const *folder = "FileMonitor";
		fs::path const self; // folder directory
		fs::path const removedPath;
		fs::path const addedPath;


		FileMonitor()
		: self(sys::GetTemporaryPath(folder))
		, removedPath(self/removedFilename)
		, addedPath(self/addedFilename)
		{
			if (self.empty())
			{
				SDL::perror(folder);
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
				Remove({self});
				future.wait();
			}
		}

		void Add(std::vector<fs::path> const &paths)
		{
			std::ostream stream(addedPath);
			for (fs::path const &path : paths)
			{
				stream << path;
			}
		}

		void Remove(std::vector<fs::path> const &paths)
		{
			std::ostream stream(removedPath);
			for (fs::path const &path : paths)
			{
				stream << path;
			}
		}

		void Thread(); // platform dependent

		using Callback = std::function<void(fs::path const &path)>;

		void Update(Callback&& onAdd, Callback&& onRemove)
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
					watched.insert(path);
					onAdd(path);
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

		void Process(fs::path const &path)
		{
			if (self < path) // update self
			{
				std::string string = path.filename();
				// Filename selects the operation
				if (string == addedFilename)
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
				if (string == removedFilename)
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
				Notify(path);
			}
		}

		void Notify(fs::path const &path)
		{
			(void) path; // TODO
		}

		void QueryError(std::exception const &exception)
		{
			SDL::SetError(CaughtException, exception.what());
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

void FileMonitor::Thread()
{
	int const fd = inotify_init();
	if (-1 == fd)
	{
		SDL::perror("inotify_init", std::strerror(errno));
		return;
	}

	std::map<fs::path, int> data;
	char buf[BUFSIZ];

	while (not done) try
	{
		// Update our list of watched directories

		Update
		(
			[&](fs::path const &path) // on add
			{
				int const wd = inotify_add_watch(fd, path.c_str(), IN_MODIFY);
				if (-1 == wd)
				{
					SDL::perror("inotify_add_watch", std::strerror(errno));
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
					SDL::perror("inotify_rm_watch", std::strerror(errno));
				}
				data.erase(path);
			}
		);

		// Block until there are events to process
		ssize_t const sz = read(fd, buf, sizeof(buf));
		if (-1 == sz)
		{
			SDL::SetError(ColonSeparator, "inotify read", std::strerror(errno));
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
			Process(fs::path(ev->name, ev->name + ev->len));
		}
	}
	catch (std::exception const &exception)
	{
		QueryError(exception);
	}
}

#else // BSD
#if defined(__BSD__)

#include <sys/event.h>

void FileWatcher::Thread()
{
	int kq = kqueue();
	if (-1 == kq)
	{
		SDL::perror("kqueue", std::strerror(errno));
		return;
	}

	std::vector<struct kevent> monitor;
	std::vector<struct kevent> events;

	while (not done) try
	{
		Update
		(
			[&](fs::path const &path) // on add
			{
				int fd = open(path.c_str(), O_EVTONLY);
				if (-1 == fd)
				{
					SDL::perror("kqueue open", std::strerror(errno));
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
			union
			{
				uintptr_t uiptr;
				void *udata
				char *str;
			};
			if (EV_ERROR == events[ev].flags)
			{
				uiptr = events[ev].data;
				SDL::perror("EV_ERROR", str);
			}
			else
			{
				udata = events[ev].udata;
				Process(str);
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

namespace
{
	std::string GetLastErrorAsString()
	{
		LPSTR buffer = nullptr;
		size_t size = FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), &buffer, 0, nullptr);

		std::string message(buffer, size);
		LocalFree(buffer);
		return message;
	}
}

void FileMonitor::Thread()
{
	std::map<fs::path, HANDLE> data;

	while (not done) try
	{
		// Update our list of directories watched

		Update
		(
			[&](fs::path const &path) // on add
			{
				HANDLE const handle = FindFirstChangeNotification(path.c_str(), TRUE, FILE_NOTIFY_CHANGE_LAST_WRITE);
				if (INVALID_HANDLE_VALUE == handle)
				{
					SDL::perror("FindFirstChangeNotification", GetLastErrorAsString());
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
					SDL::perror("FindCloseChangeNotification", GetLastErrorAsString());
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
			SDL::perror("WaitForMultipleObjects", GetLastErrorAsString());
			return;
		}
		
		HANDLE const handle = obj.at(status - WAIT_OBJECT_0);
		if (FALSE == FindNextChangeNotification(hChanged))
		{
			SDL::perror("FindNextChangeNotification", GetLastErrorAsString());
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

