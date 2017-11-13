#include "Desktop.hpp"
#include "System.hpp"
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

	constexpr auto DIR_SEPARATOR = POSIX ? ":" : ";";
	constexpr auto EXE_EXTENSION = POSIX ? "" : ".exe";
	constexpr auto EXIT_STATUS = POSIX ? "?" : "ERRORLEVEL";

	// System environment variable names as constants

	constexpr auto PATH = "PATH";
	constexpr auto HOME = "HOME";
	constexpr auto DISPLAY = "DISPLAY";
	constexpr auto VISUAL = "VISUAL";
	constexpr auto EDITOR = "EDITOR";
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
		stl::split(paths, string, DIR_SEPARATOR);
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
					stl::trim(line);
					return line;
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

	constexpr int ExitStatus(int status)
	{
		return POSIX ? WEXITSTATUS(status) : status;
	}

	int SystemCommand(std::deque<std::string> &args, std::vector<std::string> &out)
	{
		try
		{
			// Determine whether the system has a command processor
			static bool const HasProcessor = std::system(nullptr);

			if (HasProcessor)
			{
				// Write stdout to a temporary file
				fs::path path;
				{
					fs::path program = args.front();
					path = fs::temp_directory_path()/program.filename();
					args.push_back(">" + stl::quote(path));
				}
				// Execute and acquire return value
				int status;
				{
					std::string const command = stl::merge(args, " ");
					status = std::system(command.c_str());
				}
				// Read output back from temporary file
				if (not path.empty())
				{
					std::string line;
					std::ifstream istream(path);
					while (std::getline(istream, line))
					{
						out.push_back(line);
					}
				}
				// Return the exit status
				return ExitStatus(status);
			}
		}
		catch (std::exception const &exception)
		{
			SDL::perror(exception.what());
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

	int Zenity(std::deque<std::string> &args, std::vector<std::string> &out)
	{
		static std::string const path = GetZenityPath();
		if (not path.empty())
		{
			static std::string const display = xdg::GetEnv(DISPLAY);
			args.push_front(stl::param_value("--display", display));
			args.push_front(path);
			return SystemCommand(args, out);
		}
		return ~0;
	}

	int ZenityMessage(std::string const &text, std::string const &kind, std::string const &icon="")
	{
		std::deque<std::string> args;
		// The message kind
		args.push_back(kind);
		// Escaped quotes around the text
		std::string const quote = stl::quote(text);
		args.push_back(stl::param_value("--text", quote));
		// Notification type has icon option
		if (not icon.empty())
		{
			args.push_back(stl::param_value("--window-icon", icon));
		}
		// We don't expect any output
		std::vector<std::string> out;
		return Zenity(args, out);
	}

	// Select the launcher that will open the preferred program for a file

	std::string GetStartProgramPath()
	{
		std::string open = xdg::GetProgramPath("xdg-open");
		if (open.empty())
		{
			open = xdg::GetProgramPath("start");
		}
		return open;
	}
}

std::string xdg::GetEnv(std::string const &var)
{
	return std::getenv(var.c_str());
}

std::vector<std::string> xdg::GetSystemDirs()
{
	std::string const path = xdg::GetEnv(PATH);
	return SplitDirs(path);
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
			fs::directory_iterator it(path), end;
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

bool xdg::Open(std::string const &path)
{
	static std::string const open = GetStartProgramPath();
	if (not open.empty())
	{
		std::deque<std::string> args;
		args.push_back(open);
		args.push_back(path);
		std::vector<std::string> out; // dummy
		int const status = SystemCommand(args, out);
		return 0 == status;
	}
	return false;
}

bool xdg::Edit(std::string const &path)
{
	// Try with launcher first
	bool ok = xdg::Open(path);
	if (not ok)
	{
		// Next look for a visual editor like VIM
		std::string editor = xdg::GetEnv(VISUAL);
		if (editor.empty())
		{
			// Look for a basic editor last
			editor = xdg::GetEnv(EDITOR);
		}
		if (not editor.empty())
		{
			std::deque<std::string> args;
			args.push_back(editor);
			args.push_back(path);
			args.push_back("&"); // don't block
			std::vector<std::string> out; // dummy;
			ok = 0 == SystemCommand(args, out);
		}
	}
	return ok;
}

bool xdg::ShowError(std::string const &text)
{
	return 0 == ZenityMessage(text, "--error");
}

bool xdg::ShowWarning(std::string const &text)
{
	return 0 == ZenityMessage(text, "--warning");
}

bool xdg::ShowInformation(std::string const &text)
{
	return 0 == ZenityMessage(text, "--info");
}

bool xdg::ShowNotification(std::string const &text)
{
	return 0 == ZenityMessage(text, "--notification");
}

bool xdg::ShowQuestion(std::string const &text, enum xdg::Answer &answer)
{
	int status = ZenityMessage(text, "--question");
	switch (status)
	{
	case 0:
		answer = xdg::Answer::Yes;
		return true;
	case 1:
		answer = xdg::Answer::No;
		return true;
	}
	return false;
}

bool xdg::OpenFile(std::vector<std::string> &out, enum xdg::OpenFile options, std::string const &path, std::string const &title)
{
	std::deque<std::string> args;

	// Enable the file selection option
	args.push_back("--file-selection");

	// Allow custom title
	if (not title.empty())
	{
		// Escaped quotes around the title
		std::string const quote = stl::quote(title);
		args.push_back(stl::param_value("--title", quote));
	}

	// Default directory or file
	if (not path.empty())
	{
		// Escaped quotes around the path
		std::string quote = stl::quote(path);
		args.push_back(stl::param_value("--filename", quote));
	}

	// Select multiple files
	if (options & xdg::OpenFile::Multiple)
	{
		args.push_back("--multiple");
		// Use the system directory separator
		std::string quote = stl::quote(DIR_SEPARATOR);
		args.push_back(stl::param_value("--separator", quote));
	}

	// Choose a directory rather than a file
	if (options & xdg::OpenFile::Directory)
	{
		args.push_back("--directory");
	}

	// Save rather than open
	if (options & xdg::OpenFile::Save)
	{
		args.push_back("--save");
	}

	// Block on call to external program
	int const status = Zenity(args, out);
	// Separate the result string
	out = SplitDirs(out.front());
	// Okay if exit code zero
	return 0 == status;
}
