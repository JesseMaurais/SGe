/**\file
 * Constants for working operating system detection. These are compile-time
 * and are always defined for all platforms so that they work as constexpr-if
 * declaration, whereas ordinary preprocessor macros will raise a compile-time
 * error when use of a macro in a constexpr conditional is not defined.
 */
#ifndef System_hpp
#define System_hpp

#ifdef __has_include
#	if __has_include(<unistd.h>)
#		include <unistd.h>
constexpr bool UNIX = true;
#	else
constexpr bool UNIX = false;
#	endif
#	if __has_include(<windows.h>)
#		include <windows.h>
constexpr bool WIN32 = true;
#	else
constexpr bool WIN32 = false;
#	endif
#endif

#if defined(_POSIX_VERSION)
constexpr auto POSIX = _POSIX_VERSION;
#else
constexpr auto POSIX = UNIX;
#endif

#endif // file
