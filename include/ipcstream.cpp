#include "ipcstream.hpp"

#if __has_include("unistd.h")
#include <unistd.h>
#else
#if __has_include("io.h")
#include <io.h>
constexpr auto write = _write;
constexpr auto read _read;
using ssize_t = int;
#else
#error Cannot find system header
#endif
#endif

namespace io
{
	template <class Char, template <class> class Traits>
	void basic_iipcstream<Char, Traits>::execute(std::basic_string_view<Char> command, ios_base::openmode mode)
	{

	}

	// Instantiate the templates here
	
	template class basic_iipcstream<char>;
	template class basic_iipcstream<wchar_t>;

	template class basic_oipcstream<char>;
	template class basic_oipcstream<wchar_t>;

	template class basic_ipcstream<char>;
	template class basic_ipcstream<wchar_t>;
}

