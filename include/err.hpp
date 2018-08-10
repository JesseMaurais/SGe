#ifndef err_hpp
#define err_hpp

#include <sstream>
#include <string>
#include <cstdio>
#include "io.hpp"

namespace sys
{
	template <typename... Args>
	void ferror(std::string_view format, Args... args)
	{
		std::string string;
		if (::io::sprintf(string, format, args...) > 0)
		{
			std::perror(string.c_str());
		}
	}

	template <typename... Args>
	void perror(Args... args)
	{
		std::stringstream stream;
		((stream << args << " "), ...);
		if (stream)
		{
			std::perror(stream.str().c_str());
		}
	}
}

#endif // file
