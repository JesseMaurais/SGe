/**\file
 * Desktop agnostic utility functions for XDG compliant systems.
 */
#ifndef Desktop_hpp
#define Desktop_hpp

#include <string>
#include <vector>

namespace xdg
{
	// System environment variable names as string constants
	constexpr auto CURRENT_DESKTOP = "XDG_CURRENT_DESKTOP";
	constexpr auto DATA_DIRS = "XDG_DATA_DIRS";
	constexpr auto CONFIG_DIRS = "XDG_CONFIG_DIRS";
	constexpr auto CONFIG_HOME = "XDG_CONFIG_HOME";
	constexpr auto DATA_HOME = "XDG_DATA_HOME";
	constexpr auto CACHE_HOME = "XDG_CACHE_HOME";
	constexpr auto MENU_PREFIX = "XDG_MENU_PREFIX";

	/// Get system environment variable
	std::string GetEnv(std::string const &var);

	/// System PATH separated into components
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

	/// Find any application menus under the configuration directories
	std::vector<std::string> FindApplicationsMenus();

	/// Find all application desktop files under the configuration directories
	std::vector<std::string> FindDesktopApplications();
}

#endif // file
