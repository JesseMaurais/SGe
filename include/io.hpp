#ifndef io_hpp
#define io_hpp

#include <string>
#include <iostream>
#include "fmt.hpp"

namespace io
{
	template <typename... Args>
	std::string::size_type sprintf(std::string &string, std::string_view format, Args... args)
	{
		string = (fmt::format(format) % ... % args);
		return string.size();
	}

	template <typename... Args>
	std::string::size_type fprintf(std::ostream &stream, std::string_view format, Args... args)
	{
		std::string string;
		auto const subs = sprintf(string, format, args...);
		stream << string;
		return subs;
	}

	template <typename... Args>
	std::string::size_type printf(std::string_view format, Args... args)
	{
		return fprintf(std::cout, format, args...);
	}
}

#endif // file
