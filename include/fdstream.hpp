#ifndef fdstream_hpp
#define fdstream_hpp

#include <iostream>
#include "fdbuf.hpp"

namespace sys::io
{
	template <class Char, template <class> class Traits = std::char_traits>
	class basic_ifdstream : public std::basic_istream<Char, Traits<Char>>, basic_fdbuf<Char, Traits>
	{
	public:
		basic_ifdstream(int fd) : basic_fdbuf(fd), basic_istream(this)
		{}
	};

	template <class Char, template <class> class Traits = std::char_traits>
	class basic_ofdstream : public std::basic_ostream<Char, Traits<Char>>, basic_fdbuf<Char, Traits>
	{
	public:
		basic_ofdstream(int fd) : basic_fdbuf(fd), basic_ostream(this)
		{}
	};

	template <class Char, template <class> class Traits = std::char_traits>
	class basic_fdstream : public basic_ifdstream<Char, Traits>, basic_ofdstream<Char, Traits>
	{
	public:
		basic_fdstream(int in, int out) : basic_ifdstream(in), basic_ofdstream(out)
		{}
		basic_fdstream(int fd[2]) : basic_fdstream(fd[0], fd[1])
		{}
	};

	// Type aliases for ease of use
	using fdstream = basic_fdstream<char>;
	using wfdstream = basic_fdstream<wchar_t>;
}

#endif // file

