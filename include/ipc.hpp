#ifndef ipc_hpp
#define ipc_hpp

#include <string>
#include <iostream>
#include <initializer_list>
#include "file.hpp"
#include "membuf.hpp"
#include "pipebuf.hpp"

namespace sys::io
{
	template
	<
	 class Char,
	 template <class> class Traits = std::char_traits,
	 template <class> class Alloc = std::allocator,
	 template <class, class> class basic_stream,
	 sys::file::openmode default_mode
	>
	class impl_pstream
	: basic_stream<Char, Traits<Char>>
	, basic_membuf<Char, Traits, Alloc>
	, basic_pipebuf<Char, Traits>
	{
		using string = std::basic_string<Char, Traits<Char>, Alloc<Char>>;
		using string_view = std::basic_string_view<Char, Traits<Char>>;
		using initializer_list = std::initializer_list<Char*>;
		using openmode = sys::file::openmode;

	public:

		basic_pstream();
		basic_pstream(initializer_list args, openmode mode = default_mode)
		: basic_pstream()
		{
			open(args, mode | default_mode);
		}
		
		bool is_open() const;
		virtual void open(initializer_list args, openmode mode = default_mode);
		void close();

	private:

		sys::file::pipe pipe;
	};

	extern template impl_pstream<char>;
	extern template impl_pstream<wchar_t>;

	template
	<
	 class Char,
	 template <class> class Traits = std::char_traits,
	 template <class> class Alloc = std::allocator
	>
	class basic_pstream
	: public impl_pstream<Char, Traits, Alloc, std::basic_iostream, sys::file::in|sys::file::out>
	{
	public:
		using impl_pstream::impl_pstream;
	};
	
	using pstream = basic_pstream<char>;
	using wpstream = basic_pstream<wchar_t>;

	template
	<
	 class Char,
	 template <class> class Traits = std::char_traits,
	 template <class> class Alloc = std::allocator
	>
	class basic_ipstream
	: public impl_pstream<Char, Traits, Alloc, std::basic_istream, sys::file::in>
	{
	public:
		using impl_pstream::impl_pstream;
	};

	using ipstream = basic_ipstream<char>;
	using wipstream = basic_ipstream<wchar_t>;

	template 
	<
	 class Char,
	 template <class> class Traits = std::char_traits,
	 template <class> class Alloc = std::allocator
	>
	class basic_opstream
	: public impl_pstream<Char, Traits, Alloc, std::basic_ostream, sys::file::out>
	{
	public:
		using impl_pstream::impl_pstream;
	};

	using opstream = basic_opstream<char>;
	using wopstream = basic_opstream<wchar_t>;
}

#endif // file

