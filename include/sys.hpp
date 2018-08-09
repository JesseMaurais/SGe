#ifndef sys_hpp
#define sys_hpp

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//
// POSIX
//

#if __has_include(<unistd.h>)
#include <unistd.h>

namespace sys
{

// Portable Operating System Interface X
#if defined(_POSIX_VERSION)
constexpr long POSIX_VERSION = _POSIX_VERSION;
#else
constexpr long POSIX_VERSION = 0L;
#endif

// Single UNIX Specification
#if defined(_XOPEN_VERSION)
constexpr int XOPEN_VERSION = _XOPEN_VERSION;
#else
constexpr int XOPEN_VERSION = 0;
#endif

constexpr bool XSI = XOPEN_VERSION > 0;
constexpr bool POSIX = XSI or POSIX_VERSION > 0;
constexpr bool WINRT = false;

// Linux does not define/require these
#ifndef O_BINARY
#define O_BINARY 0L
#endif
#ifndef O_TEXT
#define O_TEXT 0L
#endif

constexpr auto dup2 = ::dup2;
constexpr auto open = ::open;
using ssize_t = ::ssize_t;
constexpr auto write = ::write;
constexpr auto read = ::read;
constexpr auto close = ::close;
using pid_t = ::pid_t;
constexpr auto fork = ::fork;
constexpr auto popen = ::popen;
constexpr auto pclose = ::pclose;
constexpr auto pipe = ::pipe;
constexpr auto execv = ::execv;

} // namespace sys

//
// MSVCRT
//

#else
#if __has_include(<io.h>)
#include <io.h>

namespace sys
{

constexpr bool XSI = false;
constexpr bool POSIX = false;

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#define O_APPEND _O_APPEND
#define O_BINARY _O_BINARY
#define O_CREAT  _O_CREAT
#define O_RDONLY _O_RDONLY
#define O_RDWR   _O_RDWR
#define O_TEXT   _O_TEXT
#define O_TRUNC  _O_TRUNC
#define O_WRONLY _O_WRONLY

constexpr auto dup2 = ::_dup2;
constexpr auto open = ::_open;
using ssize_t = int;
constexpr auto write = ::_write;
constexpr auto read = ::_read;
constexpr auto close = ::_close;

using pid_t = int;
// WIN32 does not have a fork
constexpr auto fork = [] { return -1; };

// UWP does not support the console
#ifndef _WINRT_DLL
constexpr bool WINRT = false;
constexpr auto popen = ::_popen;
constexpr auto pclose = ::_pclose;
constexpr auto pipe = [](int fd[2]) { return ::_pipe(fd, BUFSIZ, 0); };
#else
constexpr bool WINRT = true;
constexpr auto popen = [](char const *path, int mode) { return -1; };
constexpr auto pclose = [](int fd) { return -1; };
constexpr auto pipe = [](int fd[2]) { return -1; };
#endif // _WINRT_DLL

} // namespace sys

//
// Unknown
//

#else
#error Cannot find system header
#endif // io.h
#endif // unistd.h
#endif // file
