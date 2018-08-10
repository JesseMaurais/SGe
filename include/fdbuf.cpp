#include "fdbuf.hpp"
#include "sys.hpp"
#include "err.hpp"
#include <cstdio>

namespace sys::io
{
	template <class Char, template <class> class Traits>
	typename basic_fdbuf<Char, Traits>::size_type
	basic_fdbuf<Char, Traits>::xsputn(char_type const *s, size_type m)
	{
		auto const sz = m * sizeof (char_type);
		auto const n = sys::write(fd, s, sz);
		if (-1 == n)
		{
			sys::perror("write", fd, s, sz);
		}
		return n;
	}

	template <class Char, template <class> class Traits>
	typename basic_fdbuf<Char, Traits>::size_type
	basic_fdbuf<Char, Traits>::xsgetn(char_type *s, size_type m)
	{
		auto const sz = m * sizeof (char_type);
		auto const n = sys::read(fd, s, sz);
		if (-1 == n)
		{
			sys::perror("read", fd, s, sz);
		}
		return n;
	}

	template class basic_fdbuf<char>;
	template class basic_fdbuf<wchar_t>;
}

