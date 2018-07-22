#ifndef iobuf_hpp
#define iobuf_hpp

#include <streambuf>
#include <cstring>

namespace sys::io
{
	template
	<
	 class Char,
	 template <class> class Traits = std::char_traits
	>
	class basic_iobuf : public virtual std::basic_streambuf<Char, Traits<Char>>
	{
		using traits = typename Traits<Char>;
		using size_type = std::streamsize;

	protected:

		int_type overflow(int_type c)
		{
			constexpr int_type eof = traits::eof();
			if (pptr() == epptr())
			{
				if (-1 == sync()) c = eof;
			}
			if (traits::eq_int_type(eof, c))
			{
				setp(nullptr, nullptr);
			}
			else
			{
				*pptr() = traits::to_char_type(c);
			}
			return traits::not_eof(c);
		}

		int_type underflow()
		{
			if (gptr() == egptr())
			{
				auto const max = egptr() - eback();
				auto const sz = sgetn(eback(), max);
				if (0 < sz)
				{
					auto const diff =  max - sz;
					std::memmove(eback() + diff, eback(), sz*sizeof char_type);
					gbump(-sz);
				}
				else
				{
					setg(nullptr, nullptr, nullptr);
					return traits::eof();
				}
			}
			return traits::to_int_type(*base::gptr());
		}

		int sync()
		{
			if (pbase() != pptr())
			{
				auto const off = pptr() - pbase();
				auto const sz = sputn(pbase(), off);
				if (0 < sz)
				{
					auto const diff = off - sz;
					std::memmove(pbase(), pbase() + sz, diff*sizeof char_type);
					pbump(-sz);
				}
				return sz < 0 ? -1 : 0;
			}
			return pptr() != epptr() ? 0 : -1;
		}
	}
	
	using buf = basic_iobuf<char>;
	using wbuf = basic_iobuf<wchar_t>;
}

#endif // file
