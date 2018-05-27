/**\file
 * Constants for operating system detection. These are compile-time and are
 * always defined for all systems so that they work within any constexpr-if
 * statements, whereas ordinary preprocessor macros will raise a compile-time
 * error when use of a macro in a constexpr conditional is not defined.
 *
 * We prefer to check for features via the __has_include test macro rather
 * than rely on preprocessor symbols to detect the operating system API. So we
 * take any system with unistd.h as a UNIX and any with windows.h as a Win32
 * capable system. This does not mean they are UNIX or Windows operating
 * systems but that we can use their APIs. This is also why we have separate
 * symbols for __WINDOWS__ the OS and __WIN32__ the API. For example, Cygwin
 * has both APIs. We extend the SDL2 platform macros with
 *
 * 		__UNIX__	Has the unistd header included
 * 		__POSIX__	Is a POSIX compliant operating system
 * 		__BSD__		Derives from BSD UNIX (FreeBSD, NetBSD, OpenBSD, or MacOS)
 * 		__WIN32__	Has the windows header included
 */

#ifndef System_hpp
#define System_hpp

#include <SDL2/SDL_platform.h> // has macros

// Check which operating system headers exist

#ifdef __has_include // try headers

#if __has_include(<unistd.h>)
#undef __UNIX__
#define __UNIX__ 1 
#include <unistd.h>
constexpr bool UNIX = true;
#else
constexpr bool UNIX = false;
#endif

#if __has_include(<windows.h>)
#undef __WIN32__
#define __WIN32__ 1
#include <windows.h>
#ifndef WIN32
constexpr bool WIN32 = true;
#endif // WIN32
#else
constexpr bool WIN32 = false;
#endif

#else // fall back on macros

#if defined(unix) || defined(__unix) || defined(__unix__)
#undef __UNIX__
#define __UNIX__ 1
#include <unistd.h>
constexpr bool UNIX = true;
#else
constexpr bool UNIX = false;
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
#undef __WIN32__
#define __WIN32__ 1
#include <windows.h>
constexpr bool WIN32 = true;
#else
constexpr bool WIN32 = false;
#endif

#endif // headers

// Not every system that is Win32 capable is the Windows operating system

#if defined(__WINDOWS__)
constexpr auto WINDOWS = true;
#else
constexpr auto WINDOWS = false;
#endif

// POSIX exists in principle for any UNIX even if we lack the version

#if defined(_POSIX_VERSION)
#undef __POSIX__
#define __POSIX__ 1
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

#if defined(__MACOS__)
constexpr bool MACOS = true;
#else
constexpr bool MACOS = false;
#endif

#if defined(__FREEBSD__)
constexpr bool FREEBSD = true;
#else
constexpr bool FREEBSD = false;
#endif

#if defined(__NETBSD__)
constexpr bool NETBSD = true;
#else
constexpr bool NETBSD = false;
#endif

#if defined(__OPENBSD__)
constexpr bool OPENBSD = true;
#else
constexpr bool OPENBSD = false;
#endif

// Other BSDs do not support all that we want from a BSD

constexpr bool BSD = MACOS or FREEBSD or NETBSD or OPENBSD;

#if defined(__MACOS__) || defined(__FREEBSD__) || defined(__NETBSD__) || defined(__OPENBSD__)
#undef __BSD__
#define __BSD__ 1
#endif

// Some sanity checks. At a minimum...

static_assert(POSIX or WIN32, "Cannot determine which operating system API to use");

#endif // file
