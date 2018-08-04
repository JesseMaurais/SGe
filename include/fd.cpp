#include "fd.hpp"
#include "os.hpp"
#include "unisys.hpp"
#include <cstdio>

namespace
{
	using namespace std::ios_base;
	int convert(openmode mode)
	{
		int flags = 0;
		if (mode & in and mode & out)
		{
			flags |= O_RDWR;
		}
		else if (mode & out)
		{
			flags |= O_WRONLY;
		}
		else if (mode & in)
		{
			flags |= O_RDONLY;
		}
		if (mode & app)
		{
			flags |= O_APPEND;
		}
		if (mode & trunc)
		{
			flags |= O_TRUNC;
		}
		if constexpr (sys::linux)
		{
			flags |= O_DIRECT;
		}
	}
}

namespace sys::io
{
	template
	<
	 class Char,
	 template <class> class Traits,
	 template <class> class Alloc
	>
	basic_ifdstream(int fd)
	: basic_fdbuf(fd)
	, basic_membuf(BUFSIZ)
	, basic_istream(this)
	{ }

	template
	{
	 class Char,
	 template <class> class Traits,
	 template <class> class Alloc
	>
	void open(string_view filename_view, openmode mode)
	{
		string const filename = filename_view; // should convert UTF-8
		fd = ::open(filename.c_str(), convert(mode));
		if (NFD == fd)
		{
			std::perror(__FUNCTION__);
			return;
		}
		if (mode & out and mode & in)
		{
			setbufsz(BUFSIZ, BUFSIZ);
		}
		else if (mode & out)
		{
			setbufsz(0, BUFSIZ);
		}
		else if (mode & in)
		{
			setbufsz(BUFSIZ, 0);
		}
	}
}

