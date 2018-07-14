#include "fdbuf.hpp"
#include "unisys.hpp"
#include <algorithm>
#include <system_error>
#include <cstdio>
#include <cerrno>

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

	//
	// Put area
	//

	template <class Char, template <class> class Traits>
	typename basic_fdbuf<Char, Traits>::size_type
	xsputn(char_type const *s, size_type n)
	{
		ssize_t const sz = ::write(fd, s, n*sizeof (char_type));
		if (sz < 0) std::perror(__FUNCTION__);
		return sz;
	}

	template <class Char, template <class> class Traits>
	typename basic_fdbuf<Char, Traits>::int_type
	basic_fdbuf<Char, Traits>::overflow(int_type c)
	{
		bool const not_eof = traits_type::not_eof(c);
		if (not_eof)
		{
			*base::pptr() = traits_type::to_char_type(c);
			base::pbump(1);
		}
		return sync() ? not_eof : traits_type::eof();
	}

	//
	// Get area
	//

	template <class Char, template <class> class Traits>
	typename basic_fdbuf<Char, Traits>::size_type
	basic_fdbuf<Char, Traits>::xsgetn(char_type const *s, size_type n)
	{
		ssize_t sz = ::read(fd, s, n*sizeof (char_type));
		if (sz < 0) std::perror(__FUNCTION__);
		return sz;
	}

	template <class Char, template <class> class Traits>
	typename basic_fdbuf<Char, Traits>::int_type
	basic_fdbuf<Char, Traits>::underflow()
	{
		if (base::gptr() == base::egptr())
		{
			std::ptrdiff_t const off = base::gptr() - base::eback();
			std::copy(base::egptr() - off, base::egptr(), base::eback());
			std::ptrdiff_t const diff = base::egptr() - base::gptr();
			size_type const sz = sgetn(base::eback() + off, diff);
			if (-1 == sz)
			{
				throw std::system_error(std::errno, std::generic_category());
			}
			base::setg(base::eback(), base::eback() + off, base::eback() + off + sz);
		}
		return base::gptr() == base::egptr() ? traits_type::eof() : traits_type::to_int_type(*base::gptr());
	}

	//
	// Position
	//

	template <class Char, template <class> class Traits>
	int basic_fdbuf<Char, Traits>::sync()
	{
		if (base::pbase() != base::pptr())
		{
			std::ptrdiff_t const off = base::pptr() - base::pbase();
			size_type const sz = sgetn(base::pbase(), off * sizeof (char_type));
			if (-1 == sz)
			{
				throw std::system_error(std::errno, std::generic_category());
			}
			else if (0 < sz)
			{
				std::copy(base::pbase() + sz, base::pptr(), base::pbase());
				base::setp(base::pbase(), base::epptr());
				base::pbump(off - sz);
			}
		}
		return base::pptr() != base::epptr() ? 0 : -1;
	}

	// Instantiate the templates here
	template class basic_fdbuf<char>;
	template class basic_fdbuf<wchar_t>;
}

