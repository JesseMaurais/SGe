/**\file
 * Desktop agnostic utility functions for XDG compliant systems.
 */
#ifndef Desktop_hpp
#define Desktop_hpp

#include <string>
#include <vector>

namespace xdg
{
	/// Get system environment variable
	std::string GetEnv(std::string const &var);

	/// System PATH environment variable separated into components
	std::vector<std::string> GetSystemDirs();

	/// Get an absolute native path to an executable found in the system PATH
	std::string GetProgramPath(std::string const &name);

	/// Get the application base directory
	std::string GetBaseDir();

	/// Get the home data directory
	std::string GetDataHome();

	/// Get the system data directories
	std::vector<std::string> GetDataDirs();

	/// Get the configuration home directory
	std::string GetConfigHome();

	/// Get the system configuration directories
	std::vector<std::string> GetConfigDirs();

	/// Get the directory to store cache data
	std::string GetCacheHome();

	/// Determine whether a given file is a desktop application
	bool IsDesktop(std::string const &path);

	/// Find any application menus under the configuration directories
	std::vector<std::string> FindApplicationsMenus();

	/// Find all application desktop files under the configuration directories
	std::vector<std::string> FindDesktopApplications();

	/// Open an arbitrary file with the user's preferred application
	bool Open(std::string const &path);

	/// Open a file with the user's preferred text editor
	bool Edit(std::string const &path);

	// Following are not really XDG specifications but are just as useful

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

	enum ChooseFile { None, Multiple=1, Directory=2, Save=4 };
	/// Use the Zenity program to open a file selection dialog
	bool ChooseFile(std::vector<std::string> &out, enum ChooseFile options=None, std::string const &path="", std::string const &title="");
}

#endif // file
