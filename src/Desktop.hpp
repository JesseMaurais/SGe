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

	/// Use zenity to open a file selection dialog
	std::string OpenFileSelection(std::string const &title);
}

#endif // file
