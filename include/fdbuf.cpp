#include "fdbuf.hpp"
#include "os.hpp"

#ifdef _POSIX_VERSION
#include <unistd.h>
#else
#ifdef _UCRT
#include <io.h>
constexpr auto write = _write
constexpr auto read _read
#endif // UCRT
#endif // POSIX

namespace io
{
	fdbuf::fdbuf(int fdesc) : fd(fdesc)
	{}

	fdbuf::int_type fdbuf::overflow(int_type c)
	{
		if (not traits_type::eq_int_type(c, traits_type::eof()))
		{
			*pptr() = traits_type::to_char_type(c);
			pbump(1);
		}
		return sync() == -1 ? traits_type::eof() : traits_type::not_eof(c);
	}

	fdbuf::int_type fdbuf::underflow()
	{
		if (gptr() == egptr())
		{
			std::ptrdiff_t const off = gptr() - eback();
			std::copy(egptr() - off, egptr(), eback());
			ssize_t const sz = read(fd, eback() + off, egptr() - gptr());
			setg(eback(), eback() + off, eback() + off + std::max(0, sz));
		}
		return gptr() == egptr() ? traits_type::eof() : traits_type::to_int_type(*gptr());
	}

	fdbuf::int_type fdbuf::sync()
	{
		if (pbase() != pptr())
		{
			std::ptrdiff_t const off = pptr() - pbase();
			ssize_t const sz = write(fd, outbuf, off);
			if (0 < sz)
			{
				std::copy(pbase() + sz, pptr(), pbase());
				setp(pbase(), epptr());
				pbump(off - sz);
			}
		}
		return pptr() != epptr() ? 0 : -1;
	}
}

