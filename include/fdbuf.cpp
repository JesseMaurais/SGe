#include "fdbuf.hpp"
#include "sys.hpp"
#include <cstdio>

namespace sys::io
{
	template <class Char, template <class> class Traits>
	typename basic_fdbuf<Char, Traits>::size_type
	basic_fdbuf<Char, Traits>::xsputn(char_type const *s, size_type m)
	{
		auto const sz = m * sizeof (char_type);
		::ssize_t const n = ::write(fd, s, sz);
		if (-1 == n) std::perror(__FUNCTION__);
		return n;
	}

	template <class Char, template <class> class Traits>
	typename basic_fdbuf<Char, Traits>::size_type
	basic_fdbuf<Char, Traits>::xsgetn(char_type *s, size_type m)
	{
		auto const sz = m * sizeof (char_type);
		::ssize_t const n = ::read(fd, s, sz);
		if (-1 == n) std::perror(__FUNCTION__);
		return n;
	}

	template class basic_fdbuf<char>;
	template class basic_fdbuf<wchar_t>;
}

