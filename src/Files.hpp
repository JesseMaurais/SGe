#ifndef Files_hpp
#define Files_hpp

#include "Signal.hpp"
#include <string>

struct WatchedFile : Slot<std::string>
{
	WatchedFile(std::string const &path, Observer);
};

#endif // file
