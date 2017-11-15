#include "Desktop.hpp"
#include "System.hpp"
#include "Error.hpp"
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

	constexpr int ExitStatus(int status)
	{
		return POSIX ? WEXITSTATUS(status) : status;
	}

	// Split a string of directories according the system separator
	std::vector<std::string> SplitDirs(std::string const &string)
	{
		std::vector<std::string> dirs;
		stl::split(dirs, string, DIR_SEPARATOR);
		return dirs;
	}

	// Read all the lines from a text file
	void ReadLinesFromFile(fs::path const &path, std::vector<std::string> &out)
	{
		std::string line;
		std::ifstream istream(path);
		while (std::getline(istream, line))
		{
			out.push_back(line);
		}
	}
}

std::vector<std::string> sys::GetSystemDirs()
{
	std::string const path = std::getenv("PATH");
	return SplitDirs(path);
}

std::vector<std::string> sys::GetDataDirs()
{
	return POSIX ? { "/usr/local/share", "/usr/share" } 
	             : { std::getenv("APPDATA"), std::geten("LOCALAPPDATA") };
}

std::vector<std::string> sys::GetConfigDirs()
{
	return POSIX ? { "/etc" } : { std::getenv("COMMONPROGRAMFILES") };
}
	
std::string sys::GetProgramPath(std::string const &name)
{
	// Unless extension is given, assume it's a binary
	fs::path image = name;
	if (image.extension().empty())
	{
		image += EXE_EXTENSION;
	}
	// Search for image in system directories
	for (fs::path path : sys::GetSystemDirs())
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

std::string sys::GetTemporaryDir()
{
	static struct TemporaryDir
	{
		fs::path dir;
		std::error_code error;
		TemporaryDir(std::string const &foldername)
		{
			fs::path path = fs::temp_directory_path(error);
			if (not error)
			{
				path /= foldername;
				fs::remove_all(path, error);
				if (fs::create_directory(path, error))
				{
					dir = path;
				}
			}
		}

	} temp(String(Application));

	if (temp.error)
	{
		SDL::SetError(temp.error.message());
	}
	return temp.dir;
}

std::string sys::GetTemporaryPath(std::string const &filename)
{
	static fs::path const dir = sys::GetTemporaryDir();
	if (not dir.empty())
	{
		return dir/filename;
	}
	return dir;
}

std::string sys::GetHomeDir()
{
	std::string dir = std::getenv("HOME");
	if (dir.empty())
	{
		dir = std::getenv("USERPROFILE");
	}
	return dir;
}

std::string sys::GetBaseDir()
{
	return SDL_GetBasePath();
}

namespace
{
	// Wrapper around a system call to catch output in a text file
	int SystemCommand(std::deque<std::string> &args, std::vector<std::string> &out)
	{
		// Determine whether the system has a command processor
		static bool const HasProcessor = std::system(nullptr);

		if (HasProcessor) try
		{
			// Write stdout to a temporary file
			fs::path path = sys::GetTemporaryPath(args.front());
			args.push_back(">" + stl::quote(path));
			// Execute and acquire return value
			std::string const command = stl::merge(args, " ");
			int const status = std::system(command.c_str());
			// Read output back from temporary file
			ReadLinesFromFile(path, out);
			// Return the exit status
			return ExitStatus(status);
		}
		catch (std::exception const &exception)
		{
			SDL::perror(exception.what());
		}
		return ~0;
	}

	// Select the launcher that will open the preferred program for a file
	std::string GetLauncherPath()
	{
		std::string open = sys::GetProgramPath("xdg-open");
		if (open.empty())
		{
			open = sys::GetProgramPath("start");
		}
		return open;
	}

	// Get the next non empty line from an input stream
	bool GetNextLine(std::istream &stream, std::string &line)
	{
		while (std::getline(stream, line))
		{
			constexpr auto is_blank = [](char c) { return std::isblank(c); };
			auto it = stl::find_if_not(line, is_blank);
			if (line.end() != it) // not an empty line
			{
				if (*it != '#') // not a comment
				{
					line = line.substr(0, line.find('#')); // strip comment
					stl::trim(line); // strip whitespace
					return true;
				}
			}
		}
		return false;
	}

	// Determine whether the text file at path has the magic identifier
	bool HasMagic(std::string const &path, std::string const &magic)
	{
		std::string line;
		std::ifstream stream(path);
		while (GetNextLine(stream, line))
		{
			return magic == line;
		}
		return false;
	}
}

std::string xdg::GetDataHome()
{
	fs::path path = std::getenv("XDG_DATA_HOME");
	if (path.empty())
	{
		path = sys::GetHomeDir();
		path /= ".local/share";
	}
	return path;
}

std::vector<std::string> xdg::GetDataDirs()
{
	std::string dirs = std::getenv("XDG_DATA_DIRS");
	if (dirs.empty())
	{
		return sys::GetDataDirs();
	}
	return SplitDirs(dirs);
}

std::string xdg::GetConfigHome()
{
	fs::path path = std::getenv("XDG_CONFIG_HOME");
	if (path.empty())
	{
		path = sys::GetHomeDir();
		path /= ".config";
	}
	return path;
}

std::vector<std::string> xdg::GetConfigDirs()
{
	std::string dirs = std::getenv("XDG_CONFIG_DIRS");
	if (dirs.empty())
	{
		std::vector<std::string> dirs;
		for (fs::path path : sys::GetConfigDirs())
		{
			path /= "xdg";
			dirs.emplace_back(path);
		}
		return dirs;
	}
	return SplitDirs(dirs);
}

std::string xdg::GetCacheHome()
{
	fs::path path = std::getenv("XDG_CACHE_HOME");
	if (path.empty())
	{
		path = sys::GetHomeDir();
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
	std::string const menu_prefix = std::getenv("XDG_MENU_PREFIX");
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
				if (path.extension() == ".desktop")
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
	static std::string const open = GetLauncherPath();
	if (not open.empty())
	{
		std::deque<std::string> args;
		args.push_back(open);
		args.push_back(path);
		std::vector<std::string> out; // dummy
		return 0 == SystemCommand(args, out);
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
		std::string editor = std::getenv("VISUAL");
		if (editor.empty())
		{
			// Look for a basic editor last
			editor = std::getenv("EDITOR");
		}
		if (not editor.empty())
		{
			std::deque<std::string> args;
			args.push_back(editor);
			args.push_back(path);
			args.push_back("&"); // don't block
			std::vector<std::string> out; // dummy
			ok = 0 == SystemCommand(args, out);
		}
	}
	return ok;
}

namespace
{
	// Tools to ascertain the existence and use of Zenity or Qarma

	enum Toolkit { Unknown, GTK, QT };

	enum Toolkit GuessToolkit()
	{
		std::string const desktop = std::getenv("XDG_CURRENT_DESKTOP");

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
			path = sys::GetProgramPath(zenity);
			if (path.empty())
			{
				path = sys::GetProgramPath(qarma);
			}
			break;

		case QT:
			path = sys::GetProgramPath(qarma);
			if (path.empty())
			{
				path = sys::GetProgramPath(zenity);
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
			args.push_front(path);
			return SystemCommand(args, out);
		}
		return ~0;
	}

	int Message(std::string const &text, std::string const &kind, std::string const &icon="")
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
}

bool zen::ShowError(std::string const &text)
{
	return 0 == Message(text, "--error");
}

bool zen::ShowWarning(std::string const &text)
{
	return 0 == Message(text, "--warning");
}

bool zen::ShowInformation(std::string const &text)
{
	return 0 == Message(text, "--info");
}

bool zen::ShowNotification(std::string const &text)
{
	return 0 == Message(text, "--notification");
}

bool zen::ShowQuestion(std::string const &text, enum zen::Answer &answer)
{
	int status = Message(text, "--question");
	switch (status)
	{
	case 0:
		answer = zen::Answer::Yes;
		return true;
	case 1:
		answer = zen::Answer::No;
		return true;
	}
	return false;
}

bool zen::SelectFile(std::vector<std::string> &out, enum zen::SelectFile options, std::string const &path, std::string const &title)
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
	if (options & zen::SelectFile ::Multiple)
	{
		args.push_back("--multiple");
		// Use the system directory separator
		std::string quote = stl::quote(DIR_SEPARATOR);
		args.push_back(stl::param_value("--separator", quote));
	}

	// Choose a directory rather than a file
	if (options & zen::SelectFile ::Directory)
	{
		args.push_back("--directory");
	}

	// Save rather than open
	if (options & zen::SelectFile::Save)
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
