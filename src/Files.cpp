#include "Files.hpp"
#include "System.hpp"
#include "Manager.hpp"
#include "Error.hpp"
#include <cerrno>
#include <string>
#include <future>
#include <map>

#if defined(__LINUX__)

#include <sys/inotify.h>

class FileMonitor : public Manager<std::string>
{
private:

	std::future<int> future;
	std::map<std::string, int> wd;
	int fd;

	FileMonitor() : fd(-1)
	{
		future = std::async(Thread);
	}

	int Thread()
	{
		fd = inotify_init();
		if (0 > fd)
		{
			SDL::perror("inotify_init", std::strerror(errno));
			return EXIT_FAILURE;
		}
		if (not Initialize())
		{
			return EXIT_FAILURE;
		}

		char buf[BUFSIZ];
		while (true)
		{
			ssize_t size = read(fd, buf, sizeof(buf));
			if (0 < size)
			{
				char *ptr = buf;
				char *end = buf + BUFSIZ;

				while (ptr < end)
				{
					auto event = static_cast<inotify_event*>(ptr);
					ptr += sizeof(inotify_event) + event->len;
					Notify(event);
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
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}

	void Notify(inotify_event *event)
	{
		std::string path(event->name, event->name + event->len);
	}

protected:

	void Generate(std::vector<std::string> &paths)
	{
		for (std::string const &path : paths)
		{
			int res = inotify_add_watch(fd, path.c_str(), IN_ALL_EVENTS);
			if (0 > res)
			{
				SDL::perror("inotify_add_watch", std::strerror(errno));
			}
			else
			{
				wd.insert(path, res);
			}
		}
	}

	void Destroy(std::vector<std::string> const &paths)
	{
		for (std::string const &path : paths)
		{
			if (0 > inotify_rm_watch(fd, wd.at(path)))
			{
				SDL::perror("inotify_rm_watch", std::strerror(errno));
			}
			else
			{
				wd.erase(path);
			}
		}
	}

	bool SendUpdate() override
	{
		return false; // stub
	}

public:

	static FileMonitor &Instance()
	{
		static FileMonitor singleton;
		return singleton;
	}
};

#else // BSD

#if defined(__MACOS__) || defined(__FREEBSD__) || defined(__NETBSD__) || defined(__OPENBSD__)

#else // POSIX

#if defined(__POSIX__)

#else // WINDOWS

#if defined(__WINDOWS__)

#endif /// WINDOWS
#endif /// POSIX
#endif /// BSD
#endif /// LINUX

Resources &FileWatch::Manager()
{
	return FileMonitor::Instance();
}
