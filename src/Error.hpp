#ifndef Error_hpp
#define Error_hpp

#include "std.hpp"
#include "SDL.hpp"
#include "Strings.hpp"

namespace SDL
{
	/// Overload of SDL::LogError that takes std::string
	inline bool LogError(std::string const &prefix)
	{
		return LogError(prefix.c_str());
	}

	/// Overload of SDL::LogError that takes Strings enumeration
	inline bool LogError(enum Strings prefix)
	{
		return LogError(String(prefix));
	}

	/// Overload of SDL::perror that takes std::string
	inline bool perror(std::string const &prefix)
	{
		return perror(prefix.c_str());
	}

	/// Overload of SDL::perror that takes Strings enumeration
	inline bool perror(enum Strings prefix)
	{
		return perror(String(prefix));
	}

	/// Type-safe and format-safe version of SDL_SetError. Always returns true
	template <typename... Args>	bool SetError(std::string const &format, Args&&... args)
	{
		std::string message;
		stl::sprintf(message, format, args...);
		return 0 > SDL_SetError("%s", message.c_str());
	}

	/// Type-safe and format-safe version taking Strings enum. Always returns true
	template <typename... Args>	bool SetError(enum Strings format, Args&&... args)
	{
		return SetError(String(format), args...);
	}

	/// Type-safe and format-safe version of SDL_Log
	template <typename... Args>	void Log(std::string const &format, Args&&... args)
	{
		std::string message;
		stl::sprintf(message, format, args...);
		SDL_Log("%s", message.c_str());
	}

	// Type-safe and format-safe version taking Strings enum
	template <typename... Args>	void Log(enum Strings format, Args&&... args)
	{
		Log(String(format), args...);
	}
}

#endif // file
