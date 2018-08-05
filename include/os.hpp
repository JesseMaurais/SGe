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

// Windows Runtime Components
#if defined(_WINRT_DLL)
constexpr bool WINRT = true;
#else
constexpr bool WINRT = false;
#endif

//
// Operating Systems
//

// Microsoft Windows
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__) || defined(__CYGWIN__)
# undef __WIN32__
# define __WIN32__ 1
constexpr bool WIN32 = true;
#else
constexpr bool WIN32 = false;
#endif

// Microsoft DOS
#if defined(MSDOS) || defined(_MSDOS) || defined(__MSDOS__) || defined(__DOS__)
# undef __DOS__
# define __DOS__ 1
constexpr bool DOS = true;
#else
constexpr bool DOS = false;
#endif

// GNU
#if defined(__GNU__) || defined(__gnu_hurd__) || defined(__gnu_linux__)
# undef __GNU__
# define __GNU__
constexpr bool GNU = true;
#else
constexpr bool GNU = false;
#endif

// Linux
#if defined(linux) || defined(__linux) || defined(__linux__)
# undef __LINUX__
# define __LINUX__ 1
constexpr bool LINUX = true;
#else
constexpr bool LINUX = false;
#endif

// Google Android
#if defined(__ANDROID__)
# undef __ANDROID__
# define __ANDROID__
constexpr bool ANDROID = true;
#else
constexpr bool ANDROID = false;
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

// DragonFly
#if defined(__DragonFly__)
# undef __DRAGONFLY__
# define __DRAGONFLY__ 1
constexpr bool DRAGONFLY = true;
#else
constexpr bool DRAGONFLY = false;
#endif

// OpenVMS
#if defined(VMS) || defined(__VMS)
# undef __VMS__
# define __VMS__
constexpr bool VMS = true;
#else
constexpr bool VMS = false;
#endif

// SCO OpenServer
#if defined(M_I386) || defined(M_XENIX) || defined(_SCO_DS)
# undef __OPENSERVER__
# define __OPENSERVER__ 1
constexpr bool OPENSERVER = true;
#else
constexpr bool OPENSERVER = false;
#endif

// UnixWare
#if defined(sco) || defined(_UNIXWARE7)
# undef __UNIXWARE__
# define __UNIXWARE__
constexpr bool UNIXWARE = true;
#else
constexpr bool UNIXWARE = false;
#endif

// System V
#if defined(__sysv__) || defined(__SVR4) || defined(__svr4__)
# undef __SYSV__
# define __SYSV__ 1
constexpr bool SYSV = true;
#else
constexpr bool SYSV = false;
#endif

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
#if defined(sgi) || defined(__sgi)
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
constexpr bool BSD = MACOS or FREEBSD or NETBSD or OPENBSD or DRAGONFLY;
#endif

// Legacy UNIX
#if defined(unix) || defined(__unix) || defined(__unix__)
# undef __UNIX__
# define __UNIX__ 1
constexpr bool UNIX = true;
#else
constexpr bool UNIX = SYSV or BSD or AIX or HPUX or IRIX;
#endif

constexpr bool XSI = XOPEN_VERSION > 0;
constexpr bool POSIX = XSI or POSIX_VERSION > 0;

static_assert(POSIX or WIN32, "Cannot determine which operating system API to use");

}

#endif // file

