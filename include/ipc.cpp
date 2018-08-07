#include "ipc.hpp"
#include "sys.hpp"
#include "os.hpp"

#ifdef __WIN32__
#include <processthreadsapi.h>
#else
#include <unistd.h>
#endif

namespace sys::io
{
	template
	<
	 class Char,
	 template <class> class Traits,
	 template <class> class Alloc
	>
	basic_pstream::basic_pstream()
	: fd()
	, basic_fdstream(fd[0], fd[1])
	{ }

	template
	<
	 class Char,
	 template <class> class Traits,
	 template <class> class Alloc
	>
	void basic_pstream::open(string_view program, openmode mode)
	{
		if constexpr (sys::WIN32)
		{
			
		}
		if constexpr (sys::POSIX)
		{
			::pid_t pid = ::fork();
			if (-1 == pid)
			{
			}
			else
			if (0 == pid)
			{

			}
			else
			{
			}
		}
	}

	template
	<
	 class Char,
	 template <class> class Traits,
	 template <class> class Alloc
	>
	void basic_pstream::close()
	{
	}
	
	template class basic_pstream<char>;
	template class basic_pstream<wchar_t>;
}

