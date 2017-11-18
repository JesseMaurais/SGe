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

		std::future future;
		bool done = false;

		fs::path self;

		std::set<fs::path> watched;
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
			future = std::async([this]()
			{
				Thread(); 
			});
		}
		~FileMonitor()
		{
			if (not self.empty())
			{
				self /= "stop";
				stl::touch(self);
				future.wait();
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
				if (string == "add")
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
				if (string == "remove")
				{
					std::ifstream stream(path);
					while (std::getline(stream, string))
					{
						if (not (self < string))
						{
							removed.push_back(string);
						}
					}
				}
				else
				if (string == "stop")
				{
					done = true;
				}
				// Empty the file of its content
				auto erase = std::ios::out | std::ios::trunc;
				std::ofstream stream(path, erase);
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
			SDL::SetError(ColonSeparator, "inotify", std::strerror(errno));
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

#else // WINDOWS
#if defined(__WINDOW__)

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
				HANDLE const handle = FindFirstChangeNotification(path.c_str(), TRUE, FILE_NOTIFY_CHAGE_LAST_WRITE);
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
#endif // LINUX

