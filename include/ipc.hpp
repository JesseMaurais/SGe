#ifndef ipc_hpp
#define ipc_hpp

#include <string>
#include <iostream>
#include "fd.hpp"

namespace sys::io
{
	template <class Char, template <class> class Traits = std::char_traits>
	class basic_ipstream : public basic_ifdstream<Char, Traits>
	{
	public:

		basic_ipstream();
	};

	template <class Char, template <class> Traits = std::char_traits>
	class basic_opstream : public basic_ofdstream<Char, Traits>
	{
	public:

		basic_opstream();
	};

	template <class Char, template <class> Traits = std::char_traits>
	class basic_pstream : public basic_fdstream<Char, Traits>
	{
	public:

		basic_pstream();
	};
	
	using ipstream = basic_ipstream<char>;
	using wipstream = basic_ipstream<wchar_t>;

	using opstream = basic_opstream<char>;
	using wopstream = basic_opstream<wchar_t>;

	using pstream = basic_pstream<char>;
	using wpstream = basic_pstream<wchar_t>;

	extern template basic_ipstream<char>;
	extern template basic_ipstream<wchar_t>;

	extern template basic_opstream<char>;
	extern template basic_opstream<wchar_t>;

	extern template basic_pstream<char>;
	extern template basic_pstream<wchar_t>;
}

#endif // file

