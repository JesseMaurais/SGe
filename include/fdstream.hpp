#ifndef fdstream_hpp
#define fdstream_hpp

#include <iostream>
#include "fs.hpp"
#include "fd.hpp"
#include "fdbuf.hpp"
#include "membuf.hpp"

namespace sys::io
{
	template
	<
	 class Char,
	 template <class> class Traits = std::char_traits,
	 template <class> class Alloc = std::allocator,
	 std::ios_base::openmode Open = std::ios_base::in|std::ios_base::out
	>
	class basic_fdstream
	: public std::basic_stream<Char, Traits<Char>>
	, public basic_membuf<Char, Traits, Alloc>
	, public basic_fdbuf<Char, Traits>
	{
		using string = std::basic_string<Char, Traits<Char>, Alloc<Char>>;
		using string_view = std::basic_string_view<Char, Traits<Char>>;

	public:

		basic_ifdstream(int fd = NFD);
		basic_ifdstream(sys::file::path const &path, openmode mode = Open)
		: basic_ifdstream()
		{
			open(filename, mode | Open);
		}

		void close() { fd = NFD; }
		bool is_open() const { return fd; }
		void open(std::file::path const &path, openmode mode = in);

	private:

		auto_fd fd;
	};

	template
	<
	 class Char,
	 template <class> class Traits = std::char_traits,
	 template <class> class Alloc = std::allocator
	>
	class basic_ifdstream
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

