#include "fdbuf.hpp"
#include "unisys.hpp"
#include <cstdio>

namespace sys::io
{
	template <class Char, template <class> class Traits>
	typename basic_fdbuf<Char, Traits>::size_type
	basic_fdbuf<Char, Traits>::xsputn(char_type const *s, size_type n)
	{
		::ssize_t const sz = ::write(fd, s, n*sizeof char_type);
		if (-1 == sz) std::perror(__FUNCTION__);
		return sz;
	}

	template <class Char, template <class> class Traits>
	typename basic_fdbuf<Char, Traits>::size_type
	basic_fdbuf<Char, Traits>::xsgetn(char_type *s, size_type n)
	{
		::ssize_t sz = ::read(fd, s, n*sizeof char_type);
		if (-1 == sz) std::perror(__FUNCTION__);
		return sz;
	}

	template class basic_fdbuf<char>;
	template class basic_fdbuf<wchar_t>;
}

