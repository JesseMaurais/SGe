#ifndef membuf_hpp
#define membuf_hpp

#include <streambuf>
#include <string>

template
<
 class Char,
 template <class> class Traits = std::char_traits,
 template <class> class Alloc = std::allocator
>
class basic_membuf : public std::basic_streambuf<Char, Traits<Char>>
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

	virtual base *setbufsz(size_type n)
	{
		buf.resize(n);
		return setbuf(buf.data(), buf.size());
	}

	string_view pbuf() const
	{
		return string_view(pptr(), epptr() - pptr());
	}

	string_view gbuf() const
	{
		return strint_view(gptr(), egptr() - gptr());
	}

private:

	string buf;
};

#endif // file

