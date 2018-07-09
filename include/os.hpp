#ifndef os_hpp
#define os_hpp

// https://sourceforge.net/p/predef/wiki/OperatingSystems

namespace sys
{

//
// System Interfaces
//

// Portable Operating System Interface
#if defined(_POSIX_VERSION)
constexpr long POSIX_VERISON = _POSIX_VERSION;
#else
constexpr long POSIX_VERSION = 0L;
#endif

// Single UNIX Specification
#if defined(_XOPEN_VERSION)
constexpr int XOPEN_VERSION = _XOPEN_VERSION;
#else
constexpr int XOPEN_VERSION = 0;
#endif

//
// Operating Systems
//

// Microsoft Windows
#if defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
# undef __WIN32__
# define __WIN32__ 1
constexpr bool WIN32 = true;
#else
constexpr bool WIN32 = false;
#endif

// GNU/Linux
#if defined(linux) || defined(__linux) || defined(__linux__)
# undef __LINUX__
# define __LINUX__ 1
constexpr bool LINUX = true;
#else
constexpr bool LINUX = false;
#endif

// Apple Mac OS
#if defined(__APPLE__) && defined(__MACH__)
# undef __MACOS__
# define __MACOS__ 1
constexpr bool MACOS = true;
#else
constexpr bool MACOS = false;
#endif

// FreeBSD
#if defined(__FreeBSD__)
# undef __FREEBSD__
# define __FREEBSD__ 1
constexpr bool FREEBSD = true;
#else
constexpr bool FREEBSD = false;
#endif

// NetBSD
#if defined(__NetBSD__)
# undef __NETBSD__
# define __NETBSD__ 1
constexpr bool NETBSD = true;
#else
constexpr bool NETBSD = false;
#endif

// OpenBSD
#if defined(__OpenBSD__)
# undef __OPENBSD__
# define __OPENBSD__ 1
constexpr bool OPENBSD = true;
#else
constexpr bool OPENBSD = false;
#endif

// System V
#if defined(__sysv__) || defined(__SVR4) || defined(__svr4__)
# undef __SYSV__
# define __SYSV__ 1
constexpr bool SYSV = true;
#else
constexpr bool SYSV = false;
#undef

// Oracle Solaris
#if defined(sun) || defined(__sun)
# if defined(__SYSV__)
#  undef __SOLARIS__
#  define __SOLARIS__ 1
# else
#  undef __SUNOS__
#  define __SUNOS__ 1
# endif
constexpr bool SOLARIS = SYSV;
constexpr bool SUNOS = not SYSV;
#else
constexpr bool SOLARIS = false;
constexpr bool SUNOS = false;
#endif

// Hewlett Packard Unix
#if defined(hpux) || defined(_hpux) || defined(__hpux)
# undef __HPUX__
# define __HPUX__
constexpr bool HPUX = true;
#else
constexpr bool HPUX = false;
#endif

// SGI IRIX
#if defined(sgi) || defind(__sgi)
# undef __IRIX__
# define __IRIX__ 1
constexpr bool IRIX = true;
#else
constexpr bool IRIX = false;
#endif

// IBM AIX
#if defined(_AIX)
# undef __AIX__
# define __AIX__ 1
constexpr bool AIX = true;
#else
constexpr bool AIX = false;
#endif

// Berkeley Software Distribution
#if defined(__bsdi__)
# undef __BSD__
# define __BSD__ 1
constexpr bool BSD = true;
#else
constexpr bool BSD = MACOS or FREEBSD or NETBSD or OPENBSD;
#endif

// Legacy UNIX
#if defined(unix) || defined(__unix) || defined(__unix__)
# undef __UNIX__
# define __UNIX__ 1
constexpr bool UNIX = true;
#else
constexpr bool UNIX = SYSV or BSD or AIX or HPUX or IRIX; // not linux
#endif

constexpr bool XSI = XOPEN_VERSION > 0;
constexpr bool POSIX = XSI or POSIX_VERSION > 0;

static_assert(POSIX or WIN32, "Cannot determine which operating system API to use");

}

#endif // file

