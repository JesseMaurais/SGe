#ifndef ipcstream_hpp
#define ipcstream_hpp

#include <string>
#include <iostream>
#include "fdstream.hpp"

namespace io
{
	template <class Char, template <class> Traits = std::char_traits>
	class basic_iipcstream : public basic_ifdstream<Char, Traits>
	{
		using base = typename basic_ifdstream<Char, Traits>;
		using string_view = typename std::basic_string_view<Char, Traits<Char>>;

	public:

		void execute(string_view command, ios_base::openmode mode = ios_base::in);
	};

	template <class Char, template <class> Traits = std::char_traits>
	class basic_oipcstream : public basic_ofdstream<Char, Traits>
	{
		using base = typename basic_ofdstream<Char, Traits>;
		using string_view = typename std::basic_string_view<Char, Traits<Char>>;

	public:

		void execute(view command, ios_base::openmode mode = ios_base::in);
	};

	template <class Char, template <class> Traits = std::char_traits>
	class basic_ipcstream : public basic_fdstream<Char, Traits>
	{
		using base = typename basic_fdstream<Char, Traits>;
		using string_view = typename std::basic_string_view<Char, Traits<Char>>;

	public:

		void execute(string_view command, ios_base::openmode mode = ios_base::in);
	};

	// Hide implementation details

	extern template basic_iipcstream<char>;
	extern template basic_iipcstream<wchar_t>;

	extern template basic_oipcstream<char>;
	extern template basic_oipcstream<wchar_t>;

	extern template basic_ipcstream<char>;
	extern template basic_ipcstream<wchar_t>;

	// Type aliases for ease of use
	
	using iipcstream = basic_iipcstream<char>;
	using wiipcstream = basic_iipcstream<wchar_t>;

	using oipcstream = basic_oipcstream<char>;
	using woipcstream = basic_oipcstream<wchar_t>;

	using ipcstream = basic_ipcstream<char>;
	using wipcstream = basic_ipcstream<wchar_t>;
}

#endif // file

