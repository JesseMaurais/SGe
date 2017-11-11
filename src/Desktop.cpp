#include "Desktop.hpp"
#include "SDL.hpp"
#include "std.hpp"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <deque>
#include <set>

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

namespace
{
	// Handle some differences between POSIX and other file systems

	#ifdef _WIN32
	constexpr auto POSIX = false;
	#else
	constexpr auto POSIX = true;
	#endif
	constexpr auto DIR_SEPARATOR = POSIX ? ':' : ';';
	constexpr auto EXE_EXTENSION = POSIX ? "" : ".exe";

	// System environment variable names as constants

	constexpr auto PATH = "PATH";
	constexpr auto HOME = "HOME";
	constexpr auto CURRENT_DESKTOP = "XDG_CURRENT_DESKTOP";
	constexpr auto DATA_DIRS = "XDG_DATA_DIRS";
	constexpr auto CONFIG_DIRS = "XDG_CONFIG_DIRS";
	constexpr auto CONFIG_HOME = "XDG_CONFIG_HOME";
	constexpr auto DATA_HOME = "XDG_DATA_HOME";
	constexpr auto CACHE_HOME = "XDG_CACHE_HOME";
	constexpr auto MENU_PREFIX = "XDG_MENU_PREFIX";

	// Utility functions in this module

	std::vector<std::string> SplitDirs(std::string const &string)
	{
		std::vector<std::string> paths;
		stl::split(paths, string, stl::to_string(DIR_SEPARATOR));
		return paths;
	}

	std::string GetLine(std::istream &stream)
	{
		std::string line;
		while (std::getline(stream, line))
		{
			constexpr auto is_blank = [](char c) { return std::isblank(c); };
			auto it = stl::find_if_not(line, is_blank);
			if (line.end() != it) // not an empty line
			{
				if (*it != '#') // not a comment
				{
					return stl::trim(line);
				}
			}
		}
		return line;
	}

	bool HasMagic(std::string const &path, std::string const &magic)
	{
		std::ifstream stream(path);
		while (stream)
		{
			std::string line = GetLine(stream);
			if (not line.empty())
			{
				return magic == line;
			}
		}
		return false;
	}

	int SystemCommand(std::deque<std::string> const &args)
	{
		static bool const HasProcessor = std::system(nullptr);

		if (HasProcessor)
		{
			std::stringstream stream;
			auto it = std::ostream_iterator(stream, " ");
			stl::copy(args, it);
			std::string command = stream.str();
			return std::system(command.c_str());
		}
		return ~0;
	}

	// Tools to ascertain the existence and use of Zenity or Qarma

	enum Toolkit { Unknown, GTK, QT };

	enum Toolkit GuessToolkit()
	{
		std::string const desktop = stl::to_upper(xdg::GetEnv(CURRENT_DESKTOP));

		if (stl::find(desktop, "GNOME"))
		{
			return GTK;
		}
		if (stl::find(desktop, "XFCE"))
		{
			return GTK;
		}
		if (stl::find(desktop, "KDE"))
		{
			return QT;
		}
		return Unknown;
	}

	std::string GetZenityPath()
	{
		constexpr auto zenity = "zenity"; // preferred with GTK+
		constexpr auto qarma = "qarma"; // preferred with Qt
		// Use the desktop's toolkit to guess preference
		enum Toolkit toolkit = GuessToolkit();
		std::string path;
		switch (toolkit)
		{
		default:
		case GTK:
			path = xdg::GetProgramPath(zenity);
			if (path.empty())
			{
				path = xdg::GetProgramPath(qarma);
			}
			break;

		case QT:
			path = xdg::GetProgramPath(qarma);
			if (path.empty())
			{
				path = xdg::GetProgramPath(zenity);
			}
			break;
		}
		return path;
	}

	int Zenity(std::deque<std::string> &args)
	{
		static std::string const path = GetZenityPath();
		if (not path.empty())
		{
			args.push_front(path);
			return SystemCommand(args);
		}
		return ~0;
	}
}

std::string xdg::GetEnv(std::string const &var)
{
	return std::getenv(var.c_str());
}

std::vector<std::string> xdg::GetSystemDirs()
{
	return SplitDirs(xdg::GetEnv(PATH));
}

std::string xdg::GetProgramPath(std::string const &name)
{
	// Unless extension is given, assume it's a binary
	fs::path image = name;
	if (image.extension().empty())
	{
		image += EXE_EXTENSION;
	}
	// Search for image in system directories
	for (fs::path path : xdg::GetSystemDirs())
	{
		path /= image;
		if (fs::exists(path))
		{
			// Check executable status
			auto status = fs::status(path);
			auto permissions = status.permissions();
			permissions &= fs::perms::group_exec;
			if (fs::perms::none != permissions)
			{
				return path; // usable
			}
		}
	}
	return std::string();
}

std::string xdg::GetBaseDir()
{
	return SDL_GetBasePath();
}

std::string xdg::GetDataHome()
{
	fs::path path = xdg::GetEnv(DATA_HOME);
	if (path.empty())
	{
		path = xdg::GetEnv(HOME);
		path /= ".local/share";
	}
	return path;
}

std::vector<std::string> xdg::GetDataDirs()
{
	std::string dirs = xdg::GetEnv(DATA_DIRS);
	if (dirs.empty())
	{
		return { "/usr/local/share/", "/usr/share" };
	}
	return SplitDirs(dirs);
}

std::string xdg::GetConfigHome()
{
	fs::path path = xdg::GetEnv(CONFIG_HOME);
	if (path.empty())
	{
		path = xdg::GetEnv(HOME);
		path /= ".config";
	}
	return path;
}

std::vector<std::string> xdg::GetConfigDirs()
{
	std::string dirs = xdg::GetEnv(CONFIG_DIRS);
	if (dirs.empty())
	{
		return { "/etc/xdg" };
	}
	return SplitDirs(dirs);
}

std::string xdg::GetCacheHome()
{
	fs::path path = xdg::GetEnv(CACHE_HOME);
	if (path.empty())
	{
		path = xdg::GetEnv(HOME);
		path /= ".cache";
	}
	return path;
}

bool xdg::IsDesktop(std::string const &string)
{
	static std::set<std::string> const set = { ".desktop", ".directory" };

	fs::path const path = string;
	fs::path const extension = path.extension();

	// With no extension use magic
	if (extension.empty())
	{
		return HasMagic(path, "[Desktop Entry]");
	}
	// Base case is that the extension matches
	return set.find(extension) != set.end();
}

std::vector<std::string> xdg::FindApplicationsMenus()
{
	std::vector<std::string> paths;
	std::string const menu_prefix = xdg::GetEnv(MENU_PREFIX);
	std::string const applications_menu = menu_prefix + "applications.menu";
	for (fs::path path : xdg::GetConfigDirs())
	{
		path /= "menus";
		path /= applications_menu;
		if (fs::exists(path))
		{
			paths.push_back(path);
		}
	}
	return paths;
}

std::vector<std::string> xdg::FindDesktopApplications()
{
	std::vector<std::string> paths;
	for (fs::path path : xdg::GetConfigDirs())
	{
		path /= "applications";
		if (fs::exists(path) and fs::is_directory(path))
		{
			fs::directory_iterator it = path, end;
			while (it != end)
			{
				fs::path const &path = *it;
				if (path.extension() == "desktop")
				{
					paths.push_back(path);
				}
			}
		}
	}
	return paths;
}

std::string xdg::OpenFileSelection(std::string const &title)
{
	// Store selected file path in cache
	fs::path cache = xdg::GetCacheHome();
	cache /= "file-selection";

	std::deque<std::string> args;
	// Enable the file selection option
	args.push_back("--file-selection");
	if (not title.empty())
	{
		// Add escaped quotes around the title
		std::string quote = stl::quote(title);
		args.push_back("--title=" + quote);
	}
	// Redirect STDOUT to cache
	args.push_back(">" + cache);
	// Block on call to external program
	int status = Zenity(args);
	std::string path;
	switch (status)
	{
	case 0:
		{
		// Read the result back in
		std::istream stream(cache);
		std::getline(stream, path);
		}
		break;
	case 1:
		// no selection
		break;
	case -1:
		// error
		break;
	case ~0:
		// no dialog
		break;
	}
	return path;
}
