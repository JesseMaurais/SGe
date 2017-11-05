#ifndef Error_hpp
#define Error_hpp

#include "std.hpp"
#include "SDL.hpp"
#include "Strings.hpp"

namespace SDL
{
	template <typename... Args>
	bool perror(std::string const &prefix)
	{
		return perror(prefix.c_str());
	}

	template <typename... Args>
	bool perror(enum Strings prefix)
	{
		return perror(String(prefix));
	}

	template <typename... Args>
	bool SetError(std::string const &format, Args&&... args)
	{
		std::string message;
		io::sprintf(message, format, args...);
		return 0 > SDL_SetError("%s", message.c_str());
	}

	template <typename... Args>
	bool SetError(enum Strings format, Args&&... args)
	{
		return SetError(String(format), args...);
	}

	template <typename... Args>
	void Log(std::string const &format, Args&&... args)
	{
		std::string message;
		io::sprintf(message, format, args...);
		SDL_Log("%s", message.c_str());
	}

	template <typename... Args>
	void Log(enum Strings format, Args&&... args)
	{
		Log(String(format), args...);
	}
}

#endif // file
