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

	constexpr auto PATH = "PATH";

	constexpr auto PATH_SEPARATOR = POSIX ? ':' : ';';

	constexpr auto PROGRAM_EXTENSION = POSIX ? "" : ".exe";

	constexpr auto CURRENT_DESKTOP = "XDG_CURRENT_DESKTOP";


	std::vector<std::string> SeparatePaths(std::string const &string)
	{
		std::vector<std::string> paths;
		stl::split(paths, string, stl::to_string(PATH_SEPARATOR));
		return paths;
	}

	// Tools to ascertain the existence and use of Zenity or Qarma

	enum DesktopToolkit { GTK, QT, Unknown };

	enum DesktopToolkit GuessDesktopToolkit()
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
		enum DesktopToolkit toolkit = GuessDesktopToolkit();

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

std::vector<std::string> xdg::GetPaths()
{
	return SeparatePaths(xdg::GetEnv(PATH));
}

std::string xdg::GetBasePath()
{
	return SDL_GetBasePath();
}

std::string xdg::GetProgramPath(std::string const &name)
{
	std::string const image = name + PROGRAM_EXTENSION;

	for ( auto dir : xdg::GetPaths() )
	{
		fs::path path(dir);
		path /= name;

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

int xdg::SystemCommand(std::vector<std::string> const &args)
{
	static const bool HasProcessor = std::system(nullptr);

	if (HasProcessor)
	{
		std::stringstream stream;
		auto it = std::ostream_iterator<std::string>(stream, " ");
		std::copy(std::begin(args), std::end(args), it);
		std::string string = stream.str();
		return std::system(string.c_str());
	}
	return ~0;
}
