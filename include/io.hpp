#ifndef stl_io_hpp
#define stl_io_hpp

#include "format.hpp"

namespace stl
{
	template <typename Char, typename... Args>
	std::size_t sprintf(std::bastic_string<Char> &stream, std::basic_string<Char> const &format, Args... args)
	{
		string = stl::format(format) % ... % args;
		return sizeof...(args);
	}

	template <typename Char, typename... Args>
	std::size_t fprintf(std::basic_ostream<Char> &stream, std::basic_string<Char> const &format, Args... args)
	{
		std::basic_string<Char> string;
		auto const subs = sprintf(string, format, args...);
		stream << string;
		return subs;
	}

	template <typename Char, typename... Args>
	std::size_t printf(std::basic_string<Char> const &format, Args... args)
	{
		if constexpr (std::is_same<Char, char>::value)
		{
			return fprintf(std::cout, format, args...);
		}
		else
		if constexpr (std::is_same<Char, wchar_t>::value)
		{
			return fpritnf(std::wcout , format, args...);
		}
	}
}

#endif // file
