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
	class basic_membuf : public virtual std::basic_streambuf<Char, Traits<Char>>
	{
		using traits = typename Traits<Char>;
		using alloc = typename Alloc<Char>;
		using base = typename std::basic_streambuf<Char, traits>;
		using string = typename std::basic_string<Char, traits, alloc>;
		using string_view = typename std::basic_string_view<Char, traits>;

	public:

		basic_membuf(size_type n)
		{
			setbufsiz(n);
		}

		basic_streambuf *setbuf(char_type *s, size_type n) override
		{
			size_type const m = n / 2;
			return setbuf(s, n - m, m);
		}

		basic_streambuf *setbuf(char_type *s, size_type n, size_type m)
		{
			auto t = s + n;
			auto u = t + m;
			setg(s, t, t);
		       	setp(t, u);
			return this;
		}

		basic_streambuf *setbufsz(size_type n)
		{
			buf.resize(n);
			return setbuf(buf.data(), n);
		}

		basic_streambuf *setbufsz(size_type n, size_type m)
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

