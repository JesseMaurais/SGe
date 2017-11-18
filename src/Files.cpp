#include "Files.hpp"
#include "System.hpp"
#include "Desktop.hpp"
#include "Error.hpp"
#include <cerrno>
#include <future>
#include <vector>
#include <map>

#include "stl/filesystem.hpp"
namespace fs = stl::filesystem;

namespace
{
	class FileMonitor : public Resources
	{
	private:

		fs::path const self;
		std::vector<fs::path> added;
		std::vector<fs::path> removed;

		FileMonitor()
		{
			// Use directory for asynchronous commands
			self = sys::GetTemporaryPath("FileMonitor");
			if (self.empty())
			{
				SDL::perror("FileMonitor");
				return;
			}
			added.push_back(self);
			std::async([this]()
			{
				Thread(); 
			});
		}

		void Thread(); // platform dependent

		bool Process(fs::path const &path)
		{
			if (self < path) // update self
			{
				std::string string = path.filename();
				// Filename selects the operation
				if (string == "add")
				{
					std::ifstream stream(path);
					while (std::getline(stream, string))
					{
						if (not self < string)
						{
							added.push_back(string);
						}
					}
				}
				else
				if (string == "remove")
				{
					std::ifstream stream(path);
					while (std::getline(stream, string))
					{
						if (not self < string)
						{
							removed.push_back(string);
						}
					}
				}
				else
				if (string == "stop")
				{
					return false;
				}
				// Empty the file of its content
				auto erase = std::ios::out | std::ios::trunc;
				std::ofstream stream(path, erase);
			}
			else
			{
				Notify(path);
			}
			return true;
		}

		void Notify(fs::path const &path)
		{
			(void) path; // TODO
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

	std::map<fs::path, int> watched;
	char buf[BUFSIZ];

	while (true)
	{
		// Update our list of watched directories

		for (fs::path const &path : added)
		{
			// Remove any sub directory of added path
			auto const upper = watched.upper_bound(path);
			if (watched.end() != upper)
			{
				removed.push_back(upper->second);
			}
			// Add only if path not a sub directory
			auto const lower = watched.lower_bound(path);
			if (watched.end() == lower)
			{
				int const wd = inotify_add_watch(fd, path.c_str(), IN_MODIFY);
				if (-1 == wd)
				{
					SDL::perror("inotify_add_watch", std::strerror(errno));
				}
				else
				{
					watched[path] = wd;
				}
			}
		}

		for (fs::path const &path : removed)
		{
			auto const it = watched.find(path);
			assert(watched.end() != it);
			if (watched.end() != it)
			{
				int const wd = it->second;
				if (-1 == inotify_rm_watch(fd, wd))
				{
					SDL::perror("inotify_rm_watch", std::strerror(errno));
				}
				watched.erase(it->first);
			}
		}

		// Block until there are events to process
		ssize_t const sz = read(fd, buf, sizeof(buf));
		if (-1 == sz)
		{
			SDL::SetError(ColorSeparator, "inotify read", std::strerror(errno));
			if (SDL::ShowError(SDL_MESSAGEBOX_WARNING))
			{
				continue;
			}
			break;
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
			if (not Process(fs::path(ev->name, ev->name + ev->len)))
			{
				return;
			}
		}
	}
}

#else // WINDOWS
#if defined(__WINDOW__)

void FileMonitor::Thread()
{
	std::map<fs::path, HANDLE> watched;

	while (true)
	{
		// Update our list of directories watched

		for (fs::path const &path : added)
		{
			// Remove any sub directory of added path
			auto const upper = watched.upper_bound(path);
			if (watched.end() != upper)
			{
				removed.push_back(upper->second);
			}
			// Add only if path not a sub directory
			auto const lower = watched.lower_bound(path);
			if (watched.end() == lower)
			{
				constexpr auto notify = FILE_NOTIFY_CHAGE_LAST_WRITE;
				HANDLE const handle = FindFirstChangeNotification(path.c_str(), TRUE, notify);
				if (INVALID_HANDLE_VALUE == handle)
				{
					// GetLastError
				}
				else
				{
					watched[path] = handle;
				}
			}
		}

		for (fs::path const &path : removed)
		{
			auto const it = watched.find(path);
			assert(watched.end() != it);
			if (watched.end() != it)
			{
				if (FALSE == FindCloseChangeNotification(it->second))
				{
					// GetLastError
				}
				else
				{
					watched.erase(path);
				}
			}
		}

		std::vector<HANDLE> obj(watched.size());
		stl::transform(watched, obj, [](auto const &pair) { return pair.second; });
		DWORD const status = WaitForMultipleObjects(obj.size(), obj.data(), FALSE, INFINITE);
		if (WAIT_FAILED == status)
		{
			// GetLastError
			return;
		}
		
		HANDLE const handle = obj.at(status - WAIT_OBJECT_0);
		if (FALSE == FindNextChangeNotification(hChanged))
		{
			// GetLastError
			continue;
		}

	
	}
}

#endif // WINDOWS
#endif // LINUX

