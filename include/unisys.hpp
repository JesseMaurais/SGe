#ifndef unisys_hpp
#define unisys_hpp
#if __has_include(<unistd.h>)
#include <unistd.h>
#else
#if __has_include(<io.h>)
#include <io.h>
constexpr auto dup2 = _dup2;
using ssize_t = int;
constexpr auto write = _write;
constexpr auto read = _read;
constexpr auto close = _close;
using pid_t = int;
constexpr auto fork = [] { return -1; };
#ifndef _WINRT_DLL
constexpr auto popen = _popen;
constexpr auto pclose = _pclose;
constexpr auto pipe = [](int fd[2]) { return _pipe(fd, BUFSIZ, 0); };
#endif // _WINRT_DLL
#else
#error Cannot find system header
#endif // io.h
#endif // unistd.h
#endif // file
