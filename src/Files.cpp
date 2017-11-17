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

		FileMonitor()
		{
			std::async(Thread);
		}

		static void Thread();

		static void Notify(fs::path const &path)
		{
			(void) path;
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
	int fd = inotify_init();
	if (0 > fd)
	{
		SDL::perror("inotify_init", std::strerror(errno));
		return;
	}

	std::vector<fs::path> added;
	std::vector<fs::path> removed;
	std::map<fs::path, int> map;

	fs::path const self = sys::GetTemporaryPath("FileMonitor");
	added.push_back(self);

	char buf[BUFSIZ];
	while (true)
	{
		for (fs::path const &path : added)
		{
			int wd = inotify_add_watch(fd, path.c_str(), IN_MODIFY);
			if (0 > wd)
			{
				SDL::perror("inotify_add_watch", std::strerror(errno));
			}
			map[path] = wd;
		}

		for (fs::path const &path : removed)
		{
			int wd = map.at(path);
			if (0 > inotify_rm_watch(fd, wd))
			{
				SDL::perror("inotify_rm_watch", std::strerror(errno));
			}
			map.erase(path);
		}

		ssize_t size = read(fd, buf, sizeof(buf));
		if (0 < size)
		{
			char *ptr = buf;
			char *end = buf + BUFSIZ;

			while (ptr < end)
			{
				// Convert to address of the event data
				auto event = reinterpret_cast<inotify_event*>(ptr);
				ptr += sizeof(inotify_event) + event->len;
				// Extract the path name from the raw string
				fs::path path(event->name, event->name + event->len);
				if (self < path) // update notification
				{
					std::ifstream stream(path);
					std::string string = path.filename();
					if (string == "add")
					{
						while (std::getline(stream, string))
						{
							added.push_back(string);
						}
					}
					else
					if (string == "remove")
					{
						while (std::getline(stream, string))
						{
							removed.push_back(string);
						}
					}
					fs::remove_all(path);
				}
				else
				{
					Notify(path);
				}
			}
			continue;
		}
		else
		if (size < 0)
		{
			SDL::perror("inotify read", std::strerror(errno));
		}
		else
		{
			SDL::perror("inotify read 0 bytes");
		}
	}
}

#endif

