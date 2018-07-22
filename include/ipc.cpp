#include "ipc.hpp"
#include "unisys.hpp"

namespace io
{
	template
	<
	 class Char,
	 template <class> class Traits,
	 template <class> class Alloc
	>
	void basic_pstream::open(string_view program, openmode mode)
	{
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

