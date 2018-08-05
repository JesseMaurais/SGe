#include "fd.hpp"
#include "os.hpp"
#include "fmt.hpp"
#include "unisys.hpp"
#include <cstdio>

namespace sys::io
{
	template
	<
	 class Char,
	 template <class> class Traits,
	 template <class> class Alloc
	>
	basic_fdstream(int fd)
	: basic_fdbuf(fd)
	, basic_membuf()
	, basic_istream(this)
	{ }

	template
	{
	 class Char,
	 template <class> class Traits,
	 template <class> class Alloc
	>
	void basic_fdstream::open(string_view path, openmode mode)
	{
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

