/**\file
 * Constants for operating system detection. These are compile-time and are
 * always defined for all systems so that they work with ordinary constexpr-if
 * statements, whereas ordinary preprocessor macros will raise a compile-time
 * error when use of a macro in a constexpr conditional is not defined.
 */
#ifndef System_hpp
#define System_hpp

#include <SDL2/SDL_platform.h> // has macros

// Check which OS headers we are safe to include

#ifdef __has_include // features before macros
#if __has_include(<unistd.h>)
constexpr bool UNIX = true;
#else
constexpr bool UNIX = false;
#endif
#if __has_include(<windows.h>)
constexpr bool WINDOWS = true;
#else
constexpr bool WINDOWS = false;
#endif
#else // fall back on macros
#if defined(unix) || defined(__unix) || defined(__unix__)
constexpr bool UNIX = true;
#else
constexpr bool UNIX = false;
#endif
#if defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
constexpr bool WINDOWS = true;
#else
constexpr bool WINDOWS = false;
#endif
#endif

// POSIX exists in principle for any UNIX even if we lack the version

#if defined(_POSIX_VERSION)
constexpr auto POSIX = _POSIX_VERSION;
#else
constexpr auto POSIX = UNIX ? 1UL : 0UL;
#endif

// Most concerned about POSIX but we also may also want Linux and BSD info

#if defined(__LINUX__)
constexpr bool LINUX = true;
#else
constexpr bool LINUX = false;
#endif

// The big family of BSD UNICES

#if defined(__MACOS__) || defined(__FREEBSD__) || defined(__NETBSD__) || defined(__OPENBSD__)
constexpr bool BSD = true;
#else
constexpr bool BSD = false;
#endif

// Some sanity checks. At a minimum...

static_assert(WINDOWS or POSIX, "Cannot determine which OS API to use"); // serious

#endif // file
