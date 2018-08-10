#include "fd.hpp"
#include "sys.hpp"
#include "os.hpp"
#include <cstdio>

namespace
{
	using namespace sys::file;
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
		if (mode & bin)
		{
			flags |= O_BINARY;
		}
		if (mode & app)
		{
			flags |= O_APPEND;
		}
		if (mode & trunc)
		{
			flags |= O_TRUNC;
		}
		if constexpr (sys::LINUX)
		{
			flags |= O_DIRECT;
		}
		return flags;
	}
}

namespace sys::io
{
	template
	<
	 class Char,
	 template <class> class Traits,
	 template <class> class Alloc,
	 template <class, class> class basic_stream,
	 sys::file::openmode default_mode
	>
	impl_fdstream(int fd)
	: basic_fdbuf(fd)
	, basic_membuf()
	, basic_stream(this)
	{ }

	template
	{
	 class Char,
	 template <class> class Traits,
	 template <class> class Alloc,
	 template <class, class> class basic_stream,
	 sys::file::openmode default_mode
	>
	void impl_fdstream::open(string_view view, openmode mode)
	{
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
		else
		{
			return; // error?
		}
		std::string const path = sys::file::absolute(view).string();
		fd = sys::open(path.c_str(), convert(mode));
		if (not fd)
		{
			sys::perror("open", path, mode);
			return;
		}
	}
}

