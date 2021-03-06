#ifndef Error_hpp
#define Error_hpp

#include "io.hpp"
#include "SDL.hpp"
#include "Strings.hpp"
#include <system_error>
#include <exception>

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

	/// Overload of SDL::SetErrno that takes std::string
	inline bool SetErrno(std::string const &prefix)
	{
		return SetErrno(prefix.c_str());
	}

	/// Overload of SDL::SetErrno that takes Strings enumeration
	inline bool SetErrno(enum Strings prefix)
	{
		return SetErrno(String(prefix));
	}

	/// Set error string with current errno and log it like perror does
	inline bool perror(std::string const &prefix)
	{
		return SDL::SetErrno() and SDL::LogError(prefix);
	}

	/// Type-safe and format-safe version of SDL_SetError. Always returns true
	template <typename... Args> bool SetError(std::string const &format, Args&&... args)
	{
		std::string message;
		io::sprintf(message, format, args...);
		return 0 > SDL_SetError("%s", message.c_str());
	}

	/// Type-safe and format-safe version taking Strings enum. Always returns true
	template <typename... Args> bool SetError(enum Strings format, Args&&... args)
	{
		return SetError(String(format), args...);
	}

	/// Set error string with given std::exception
	inline bool SetError(std::exception const except)
	{
		std::string const message = except.what();
		return SetError(message);
	}

	/// Set error string with given std::error_code
	inline bool SetError(std::error_code const error_code)
	{
		std::string const message = error_code.message();
		return SetError(message);
	}

	/// Set error string with given std::errc
	inline bool SetError(std::errc const errc)
	{
		std::error_code const error_code = std::make_error_code(errc);
		return SetError(error_code);
	}

	/// Type-safe and format-safe version of SDL_Log
	template <typename... Args> void Log(std::string const &format, Args&&... args)
	{
		std::string message;
		io::sprintf(message, format, args...);
		SDL_Log("%s", message.c_str());
	}

	// Type-safe and format-safe version taking Strings enum
	template <typename... Args> void Log(enum Strings format, Args&&... args)
	{
		Log(String(format), args...);
	}
}

#endif // file
