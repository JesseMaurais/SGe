#ifndef fd_hpp
#define fd_hpp

#include <iostream>
#include "fdbuf.hpp"
#include "membuf.hpp"

namespace sys::io
{
	template
	<
	 class Char,
	 template <class> class Traits = std::char_traits,
	 template <class> class Alloc = std::allocator
	>
	class basic_ifdstream
       	: public std::basic_istream<Char, Traits<Char>>
	, public basic_membuf<Char, Traits, Alloc>
	, public basic_fdbuf<Char, Traits>
	{
	public:
		basic_ifdstream(int fd)
		: basic_fdbuf(fd)
		, basic_istream(this)
		{ }
	};

	template
	<
	 class Char,
	 template <class> class Traits = std::char_traits,
	 template <class> class Alloc = std::allocator
	>
	class basic_ofdstream
       	: public std::basic_ostream<Char, Traits<Char>>
	, public basic_membuf<Char, Traits, Alloc>
	, public basic_fdbuf<Char, Traits>
	{
	public:
		basic_ofdstream(int fd)
		: basic_fdbuf(fd)
		, basic_ostream(this)
		{ }
	};

	template
	<
	 class Char,
	 template <class> class Traits = std::char_traits,
	 template <class> class Alloc = std::allocator
	>
	class basic_fdstream
       	: public basic_ifdstream<Char, Traits, Alloc>
	, public basic_ofdstream<Char, Traits, Alloc>
	{
	public:
		basic_fdstream(int in, int out)
		: basic_ifdstream(in)
		, basic_ofdstream(out)
		{ }
		basic_fdstream(int fd[2])
		: basic_fdstream(fd[0], fd[1])
		{ }
	};

	using fdstream = basic_fdstream<char>;
	using wfdstream = basic_fdstream<wchar_t>;
}

#endif // file

