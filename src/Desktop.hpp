/**\file
 * Desktop agnostic utility functions for XDG compliant systems.
 */

#ifndef Desktop_hpp
#define Desktop_hpp

#include <string>
#include <vector>
#include <map>

namespace sys
{
	/// Get name of the current user
	std::string GetCurrentUserName();

	/// System PATH environment variable separated into components
	std::vector<std::string> GetSystemDirs();

	/// Get the application base directory
	std::string GetBaseDir();

	/// Get an absolute path to the user's home directory
	std::string GetHomeDir();

	/// Get absolute paths to folders for sharing application data
	std::vector<std::string> GetSharedDirs();

	/// Get absolute paths to folders for configuration data
	std::vector<std::string> GetConfigDirs();

	/// Get an absolute path to an executable found in the system PATH
	std::string GetProgramPath(std::string const &program);

	/// Get an absolute path to a folder for temporary files
	std::string GetTemporaryDir();

	/// Get an absolute path to a temporary file name
	std::string GetTemporaryPath(std::string const &filename);

	using ini = std::map<std::string, std::map<std::string, std::string>>;
	/// Get the content of a configuration file
	bool LoadConfigs(std::string const &path, ini &configs, bool replace=true);
}

namespace xdg
{
	/// Get the home data directory
	std::string GetDataHome();

	/// Get the shared data directories
	std::vector<std::string> GetDataDirs();

	/// Get the configuration home directory
	std::string GetConfigHome();

	/// Get the system configuration directories
	std::vector<std::string> GetConfigDirs();

	/// Get the directory to cache data
	std::string GetCacheHome();

	/// Get directories to search for icons
	std::vector<std::string> GetIconDirs();

	/// Determine whether a given path is a desktop application file
	bool IsDesktop(std::string const &path);

	/// Find any application menus under the configuration directories
	std::vector<std::string> FindApplicationMenus();

	/// Find all desktop application files under the configuration directories
	std::vector<std::string> FindDesktopApplications();

	/// Open an arbitrary file with the user's preferred application
	bool Open(std::string const &path);

	/// Open a file with the user's preferred text editor
	bool Edit(std::string const &path);
}

namespace zen
{
	/// Open an error message dialog with one "OK" button
	bool ShowError(std::string const &text);

	/// Open a warning message dialog with one "OK" button
	bool ShowWarning(std::string const &text);

	/// Open an information message dialog with one "OK" button
	bool ShowInformation(std::string const &text);

	/// Open a desktop notification
	bool ShowNotification(std::string const &text);

	enum Answer { No, Yes };
	/// Open a question dialog with "No" and "Yes" buttons
	bool ShowQuestion(std::string const &text, enum Answer &answer);

	enum SelectFile { None, Multiple=1, Directory=2, Save=4 };
	/// Open a file selection dialog
	bool SelectFile(std::vector<std::string> &out, enum SelectFile opt=None, std::string const &path="", std::string const &title="");
}

#endif // file
