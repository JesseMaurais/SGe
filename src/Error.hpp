#ifndef Error_hpp
#define Error_hpp

#include "std.hpp"
#include "SDL.hpp"
#include "Strings.hpp"

template <typename... Args>
std::string::size_type Format(std::string &string, const char *format, Args... args)
{
	std::string::size_type const subs = io::sprintf(string, format, args...);
	SDL_assert(sizeof...(args) == subs);
	return subs;
}

template <typename... Args>
std::string::size_type Format(std::string &string, enum Strings format, Args... args)
{
	return Format(string, String(format), args...);
}

template <typename FormatType, typename... Args>
signed SetError(FormatType const format, Args... args)
{
	std::string string;
	Format(string, format, args...);
	return SDL_SetError("%s", string.c_str());
}

template <typename FormatType, typename... Args>
signed Log(FormatType const format, Args... args)
{
	std::string string;
	Format(string, format, args...);
	return SDL_Log("%s", string.c_str());
}

#endif // file
