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
		using base = std::basic_streambuf<Char, Traits<Char>>;
		using char_type = typename base::char_type;
		using traits_type = typename base::traits_type;
		using int_type = typename base::int_type;
		using pos_type = typename base::pos_type;
		using off_type = typename base::off_type;

	protected:

		int_type overflow(int_type c)
		{
			constexpr int_type eof = traits_type::eof();
			if (base::pptr() == base::epptr())
			{
				if (-1 == sync()) c = eof;
			}
			if (traits_type::eq_int_type(eof, c))
			{
				base::setp(nullptr, nullptr);
			}
			else
			{
				*base::pptr() = traits_type::to_char_type(c);
			}
			return traits_type::not_eof(c);
		}

		int_type underflow()
		{
			if (base::gptr() == base::egptr())
			{
				auto const max = base::egptr() - base::eback();
				auto const sz = base::sgetn(base::eback(), max);
				if (0 < sz)
				{
					auto const diff =  max - sz;
					std::memmove(base::eback() + diff, base::eback(), sz*sizeof(char_type));
					base::gbump(-sz);
				}
				else
				{
					base::setg(nullptr, nullptr, nullptr);
					return traits_type::eof();
				}
			}
			return traits_type::to_int_type(*base::gptr());
		}

		int sync()
		{
			if (base::pbase() != base::pptr())
			{
				auto const off = base::pptr() - base::pbase();
				auto const sz = base::sputn(base::pbase(), off);
				if (0 < sz)
				{
					auto const diff = off - sz;
					std::memmove(base::pbase(), base::pbase() + sz, diff*sizeof(char_type));
					base::pbump(-sz);
				}
				return sz < 0 ? -1 : 0;
			}
			return base::pptr() != base::epptr() ? 0 : -1;
		}
	};
	
	using buf = basic_iobuf<char>;
	using wbuf = basic_iobuf<wchar_t>;
}

#endif // file
