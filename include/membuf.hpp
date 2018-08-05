#ifndef membuf_hpp
#define membuf_hpp

#include <streambuf>
#include <string>

namespace sys::io
{
	template
	<
	 class Char,
	 template <class> class Traits = std::char_traits,
	 template <class> class Alloc = std::allocator
	>
	class basic_membuf : public std::basic_streambuf<Char, Traits<Char>>
	{
		using base = std::basic_streambuf<Char, Traits<Char>>;
		using string = std::basic_string<Char, Traits<Char>, Alloc<Char>>;
		using string_view = std::basic_string_view<Char, Traits<Char>>;
		using size_type = typename base::streamsize;
		using char_type = typename base::char_type;

	public:

		basic_membuf(size_type n)
		{
			setbufsiz(n);
		}

		base *setbuf(char_type *s, size_type n) override
		{
			size_type const m = n / 2;
			return setbuf(s, n - m, m);
		}

		base *setbuf(char_type *s, size_type n, size_type m)
		{
			auto t = s + n;
			auto u = t + m;
			setg(s, t, t);
			setp(t, u);
			return this;
		}

		base *setbufsz(size_type n)
		{
			buf.resize(n);
			return setbuf(buf.data(), n);
		}

		base *setbufsz(size_type n, size_type m)
		{
			buf.resize(n + m);
			return setbuf(buf.data(), n, m);
		}

		string_view pview() const
		{
			return string_view(pbase(), pptr() - pbase());
		}

		string_view gview() const
		{
			return string_view(pptr(), egptr() - pptr());
		}

	private:

		string buf;
	};
}

#endif // file

