#ifndef Files_hpp
#define Files_hpp

#include "Signal.hpp"
#include <string>

namespace sys::file
{
	struct Notify : Slot<std::string>
	{
		Notify(std::string_view path, Observer);
	};
}

#endif // file
