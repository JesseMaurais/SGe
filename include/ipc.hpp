#ifndef ipc_hpp
#define ipc_hpp

#include <string>
#include <iostream>
#include "fdstream.hpp"

namespace sys::io
{
	template
	<
	 class Char,
	 template <class> class Traits = std::char_traits,
	 template <class> class Alloc = std::allocator
	>
	class basic_pstream : public basic_fdstream<Char, Traits, Alloc>
	{
		using string_view = typename basic_string_view<Char, Traits<Char>>;
		using namespace std::ios_base;

	public:

		basic_pstream();
		basic_pstream(string_view program, openmode mode = in|out)
		{
			open(program, mode);
		}
		~basic_pstream()
		{
			close();
		}
		
		bool is_open() const;
		virtual void open(string_view program, openmode mode = in|out);
		void close();
	};

	using pstream = basic_pstream<char>;
	using wpstream = basic_pstream<wchar_t>;

	extern template basic_pstream<char>;
	extern template basic_pstream<wchar_t>;

	template
	<
	 class Char,
	 template <class> class Traits = std::char_traits,
	 template <class> class Alloc = std::allocator
	>
	class basic_ipstream : public basic_pstream<Char, Traits, Alloc>
	{
	public:

		basic_ipstream();
		basic_ipstream(string_view program, openmode mode = in)
		: basic_pstream(program mode)
		{ }

		void open(string_view program, openmode mode = in) override
		{
			basic_pstream::open(program, mode);
		}
	};
	
	using ipstream = basic_ipstream<char>;
	using wipstream = basic_ipstream<wchar_t>;

	extern template basic_ipstream<char>;
	extern template basic_ipstream<wchar_t>;

	template 
	<
	 class Char,
	 template <class> class Traits = std::char_traits,
	 template <class> class Alloc = std::allocator
	>
	class basic_opstream : public basic_pstream<Char, Traits, Alloc>
	{
	public:

		basic_opstream();
		basic_opstream(string_view program, openmode mode = out)
		: basic_pstream(program, mode)
		{ }

		void open(string_view program, openmode mode = out) override
		{
			basic_pstream::open(program, mode);
		}
	};

	using opstream = basic_opstream<char>;
	using wopstream = basic_opstream<wchar_t>;

	extern template basic_opstream<char>;
	extern template basic_opstream<wchar_t>;
}

#endif // file

