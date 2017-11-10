#include "Desktop.hpp"
#include "SDL.hpp"
#include "std.hpp"
#include <cstdlib>
#include <sstream>

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

namespace
{
#ifdef _WIN32
	constexpr auto POSIX = false;
#else
	constexpr auto POSIX = true;
#endif

	constexpr auto DIR_SEPARATOR = POSIX ? ':' : ';';
	constexpr auto PROGRAM_EXTENSION = POSIX ? "" : ".exe";

	constexpr auto PATH = "PATH";
	constexpr auto HOME = "HOME";

	std::vector<std::string> SplitDirs(std::string const &string)
	{
		std::vector<std::string> paths;
		stl::split(paths, string, stl::to_string(DIR_SEPARATOR));
		return paths;
	}

	int SystemCommand(std::vector<std::string> const &args)
	{
		static const HasProcessor = std::system(nullptr);

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

	enum Toolkit { GTK, QT, Unknown };

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
		enum Toolkit toolkit = GuessToolkit();

		constexpr auto zenity = "zenity"; // preferred with GTK+
		constexpr auto qarma = "qarma"; // preferred with Qt

		std::string path;
		switch (toolkit)
		{
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
}

std::string xdg::GetEnv(std::string const &var)
{
	return std::getenv(var.c_str());
}

std::vector<std::string> xdg::GetSystemDirs()
{
	return SplitPaths(xdg::GetEnv(PATH));
}

std::string xdg::GetProgramPath(std::string const &name)
{
	fs::path image = name;
	if (image.extension().empty())
	{
		image /= PROGRAM_EXTENSION;
	}

	for (auto dir : xdg::GetSystemDirs())
	{
		fs::path path(dir);
		path /= image;

		if (fs::exists(path))
		{
			auto status = fs::status(path);
			auto permissions = status.permissions();
			permissions &= fs::perms::group_exec;
			if (fs::perms::none != permissions)
			{
				return path;
			}
		}
	}
	return std::string();
}

std::string xdg::GetBaseDir()
{
	return SDL_GetBaseDir();
}

std::string xdg::GetDataHome()
{
	fs::path path = xdg::GetEnv(xdg::DATA_HOME);
	if (path.empty())
	{
		path = xdg::GetEnv(HOME);
		path /= ".local/share";
	}
	return path;
}

std::vector<std::string> xdg::GetDataDirs()
{
	std::string dirs = xdg::GetEnv(xdg::DATA_DIRS);
	if (dirs.empty())
	{
		return { "/usr/local/share/", "/usr/share" };
	}
	return SplitDirs(dirs);
}

std::string xdg::GetConfigHome()
{
	fs::path path = xdg::GetEnv(xdg::CONFIG_HOME);
	if (path.empty())
	{
		path = xdg::GetEnv(HOME);
		path /= ".config";
	}
	return path;
}

std::vector<std::string> xdg::GetConfigDirs()
{
	std::string dirs = xdg::GetEnv(xdg::CONFIG_DIRS);
	if (dirs.empty())
	{
		return { "/etc/xdg" };
	}
	return SplitDirs(dirs);
}

std::string xdg::GetCacheHome()
{
	fs::path path = xdg::GetEnv(xdg::CACHE_HOME);
	if (path.empty())
	{
		path = xdg::GetEnv(HOME);
		path /= ".cache";
	}
	return path;
}

std::vector<std::string> FindApplicationsMenus()
{
	std::vector<std::strings> paths;
	std::string const menu_prefix = xdg::GetEnv(xdg::MENU_PREFIX);
	std::string const applications_menu = menu_prefix + "applications.menu";
	for (fs::path dir : xdg::GetConfigDirs())
	{
		dir /= "menus";
		fs::path path = dir/applications_menu;
		if (fs::exists(path))
		{
			paths.push_back(path);
		}
	}
	return paths;
}

std::vector<std::string> FindDesktopApplications()
{
	std::vector<std::string> paths;
	for (fs::path dir : xdg::GetConfigDirs())
	{
		dir /= "applications";
		if (fs::exists(dir) and fs::is_directory(dir))
		{
			fs::directory_iterator it = dir, end;
			while (it != end)
			{
				fs::path path = *it;
				if (path().extension() == "desktop")
				{
					paths.push_back(path);
				}
			}
		}
	}
	return paths;
}

