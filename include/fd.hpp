#ifndef fd_hpp
#define fd_hpp

#include <iostream>
#include "file.hpp"
#include "fdbuf.hpp"
#include "membuf.hpp"

namespace sys::io
{
	template
	<
	 class Char,
	 template <class> class Traits,
	 template <class> class Alloc,
	 template <class, class> class basic_stream,
	 sys::file::openmode default_mode
	>
	class impl_fdstream
	: public basic_stream<Char, Traits<Char>>
	, public basic_membuf<Char, Traits, Alloc>
	, public basic_fdbuf<Char, Traits>
	{
		using string = std::basic_string<Char, Traits<Char>, Alloc<Char>>;
		using string_view = std::basic_string_view<Char, Traits<Char>>;
		using openmode = sys::file::openmode;

	public:

		impl_fdstream(int fd = -1);
		impl_fdstream(string_view path, openmode mode = default_mode)
		: impl_fdstream()
		{
			open(path, mode | default_mode);
		}

		void close() { fd = -1; };
		bool is_open() const { return fd; }
		void open(string_view path, openmode mode = default_mode);

	private:

		sys::file::descriptor fd;
	};

	extern template impl_fdstream<char>;
	extern template impl_fdstream<wchar_t>;

	template
	<
	 class Char,
	 template <class> class Traits = std::char_traits,
	 template <class> class Alloc = std::allocator
	>
	class basic_fdstream
	: public impl_fdstream<Char, Traits, Alloc, std::basic_iostream, sys::file::in|sys::file::out>
	{
	public:
		using impl_fdstream::impl_fdstream;
	};

	using fdstream = basic_fdstream<char>;
	using wfdstream = basic_fdstream<wchar_t>;

	template
	<
	 class Char,
	 template <class> class Traits = std::char_traits,
	 template <class> class Alloc = std::allocator
	>
	class basic_ifdstream
	: public impl_fdstream<Char, Traits, Alloc, std::basic_istream, sys::file::in>
	{
	public:
		using impl_fdstream::impl_fdstream;
	};

	using ifdstream = basic_ifdstream<char>;
	using wifdstream = basic_ifstream<wchar_t>;

	template
	<
	 class Char,
	 template <class> class Traits = std::char_traits,
	 template <class> class Alloc = std::allocator
	>
	class basic_ofdstream
	: public impl_fdstream<Char, Traits, Alloc, std::basic_ostream, sys::file::out>
	{
	public:
		using impl_fdstream::impl_fdstream;
	};

	using ofdstream = basic_ofdstream<char>;
	using wofdstream = basic_ofdstream<wchar_t>;
}

#endif // file

