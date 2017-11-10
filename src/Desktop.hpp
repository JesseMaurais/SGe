/**\file
 * Desktop integration utility functions.
 */
#ifndef Desktop_hpp
#define Desktop_hpp

#include <string>
#include <vector>

namespace xdg
{
	// Get system environment variable
	std::string GetEnv(std::string const &var);

	// System PATH separated
	std::vector<std::string> GetPaths();

	// Application base path
	std::string GetBasePath();

	// Get an absolute native path to an executable found in the system PATH
	std::string GetProgramPath(std::string const &name);

	// Block execution while running an external program and return its error code
	int SystemCommand(std::vector<std::string> const &args);

	// Open an external dialog to select a file
	std::string FileSelection(std::string const &title);
}

#endif // file
