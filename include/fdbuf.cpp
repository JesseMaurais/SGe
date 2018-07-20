#include "fdbuf.hpp"
#include "unisys.hpp"
#include <system_error>
#include <cstdio>
#include <cerrno>
#include <cstring>

namespace sys::io
{
	template <class Char, template <class> class Traits>
	basic_fdbuf<Char, Traits, Alloc>::basic_fdbuf(int fd)
	{
		setfd(fd);
	}

	template <class Char, template <class> class Traits>
	void basic_fdbuf<Char, Traits, Alloc>::setfd(int fd)
	{
		this->fd = fd;
	}

	template <class Char, template <class> class Traits>
	typename basic_fdbuf<Char, Traits>::base*
	basic_fdbuf<Char, Traits>::setbuf(char_type *s, size_type n)
	{
		auto m = n / 2;
		auto t = s + m;
		setg(s, t, t);
		setp(t, s + n);
		return this;
	}

	//
	// Put
	//

	template <class Char, template <class> class Traits>
	typename basic_fdbuf<Char, Traits>::size_type
	basic_fdbuf<Char, Traits>::xsputn(char_type const *s, size_type n)
	{
		ssize_t const sz = ::write(fd, s, n*sizeof char_type);
		if (-1 == sz) std::perror(__FUNCTION__);
		return sz;
	}

	template <class Char, template <class> class Traits>
	typename basic_fdbuf<Char, Traits>::int_type
	basic_fdbuf<Char, Traits>::overflow(int_type c)
	{
		int nsync = -1;
		if (not traits::eq_int_type(traits::eof(), c))
		{
			bool const space = base::pptr() != base::epptr();
			if (not space) nsync = sync();
			*base::pptr() = traits::to_char_type(c);
			if (space) nsync = sync();
		}
		return nsync ? traits::eof() : traits::not_eof(c);
	}

	//
	// Get
	//

	template <class Char, template <class> class Traits>
	typename basic_fdbuf<Char, Traits>::size_type
	basic_fdbuf<Char, Traits>::xsgetn(char_type *s, size_type n)
	{
		ssize_t sz = ::read(fd, s, n*sizeof char_type);
		if (-1 == sz) std::perror(__FUNCTION__);
		return sz;
	}

	template <class Char, template <class> class Traits>
	typename basic_fdbuf<Char, Traits>::int_type
	basic_fdbuf<Char, Traits>::underflow()
	{
		int_type res = traits::eof();
		if (base::gptr() == base::egptr())
		{
			std::ptrdiff_t const max = base::egptr() - base::eback();
			size_type const sz = sgetn(base::eback(), max);
			if (0 < sz)
			{
				std::ptrdiff_t diff =  max - sz;
				std::memmove(base::eback() + sz, base::eback(), diff*sizeof char_type);
				base::setg(base::eback(), base::eback(), base::egptr());
				base::gbump(sz);
				char_type const c = *base::gptr();
				res = traits::to_int_type(c);
			}
		}
		return res
	}

	//
	// Pos
	//

	template <class Char, template <class> class Traits>
	int basic_fdbuf<Char, Traits>::sync()
	{
		if (base::pbase() != base::pptr())
		{
			std::ptrdiff_t const off = base::pptr() - base::pbase();
			size_type const sz = sputn(base::pbase(), off);
			if (0 < sz)
			{
				std::ptrdiff_t const diff = off - sz;
				std::memmove(base::pbase(), base::pbase() + sz, diff*sizeof char_type);
				base::setp(base::pbase(), base::epptr());
				base::pbump(diff);
			}
			return sz < 0 ? -1 : 0;
		}
		return base::pptr() != base::epptr() ? 0 : -1;
	}

	// Impl
	
	template class basic_fdbuf<char>;
	template class basic_fdbuf<wchar_t>;
}

