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
		size_type const m = n / 2;
		return setbuf(s, n - m, m);
	}

	template <class Char, template <class> class Traits>
	typename basic_fdbuf<Char, Traits>::base*
	basic_fdbuf<Char, Traits>::setbuf(char_type *s, size_type n, size_type m)
	{
		chat_type const *t = s + n;
		setg(s, t, n);
		setp(t, m);
		return this;
	}

	//
	// Put
	//

	template <class Char, template <class> class Traits>
	typename basic_fdbuf<Char, Traits>::size_type
	basic_fdbuf<Char, Traits>::xsputn(char_type const *s, size_type n)
	{
		::ssize_t const sz = ::write(fd, s, n*sizeof char_type);
		if (-1 == sz) std::perror(__FUNCTION__);
		return sz;
	}

	template <class Char, template <class> class Traits>
	typename basic_fdbuf<Char, Traits>::int_type
	basic_fdbuf<Char, Traits>::overflow(int_type c)
	{
		constexpr int_type eof = traits::eof();
		if (base::pptr() == base::epptr())
		{
			if (-1 == sync()) c = eof;
		}
		if (traits::eq_int_type(eof, c))
		{
			base::setp(nullptr, nullptr);
		}
		else
		{
			*base::pptr() = traits::to_char_type(c);
		}
		return traits::not_eof(c);
	}

	//
	// Get
	//

	template <class Char, template <class> class Traits>
	typename basic_fdbuf<Char, Traits>::size_type
	basic_fdbuf<Char, Traits>::xsgetn(char_type *s, size_type n)
	{
		::ssize_t sz = ::read(fd, s, n*sizeof char_type);
		if (-1 == sz) std::perror(__FUNCTION__);
		return sz;
	}

	template <class Char, template <class> class Traits>
	typename basic_fdbuf<Char, Traits>::int_type
	basic_fdbuf<Char, Traits>::underflow()
	{
		if (base::gptr() == base::egptr())
		{
			std::ptrdiff_t const max = base::egptr() - base::eback();
			size_type const sz = sgetn(base::eback(), max);
			if (0 < sz)
			{
				std::ptrdiff_t diff =  max - sz;
				std::memmove(base::eback() + diff, base::eback(), sz*sizeof char_type);
				base::gbump(-sz);
			}
			else
			{
				setg(nullptr, nullptr, nullptr);
				return traits::eof();
			}
		}
		return traits::to_int_type(*base::gptr());
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
				base::pbump(-sz);
			}
			return sz < 0 ? -1 : 0;
		}
		return base::pptr() != base::epptr() ? 0 : -1;
	}

	// Impl
	
	template class basic_fdbuf<char>;
	template class basic_fdbuf<wchar_t>;
}

