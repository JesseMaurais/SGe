#include "ipc.hpp"
#include "sys.hpp"
#include "err.hpp"
#include "os.hpp"
#include "fmt.hpp"

namespace sys::io
{
	template
	<
	 class Char,
	 template <class> class Traits,
	 template <class> class Alloc,
	 template <class, class> class basic_stream,
	 sys::file::openmode default_mode
	>cmd
	impl_pstream::impl_pstream()
	{ }

	template
	<
	 class Char,
	 template <class> class Traits,
	 template <class> class Alloc,
	 template <class, class> class basic_stream,
	 sys::file::openmode default_mode
	>
	void basic_pstream::open(initializer_list args, openmode mode)
	{
		if constexpr (sys::WIN32)
		{
			// do first because of Cygwin's slow fork
		}
		else
		if constexpr (sys::POSIX)
		{
			sys::file::pipe in, out;
			if (not in or not out)
			{
				return;
			}
			sys::pid_t const pid = sys::fork();
			if (-1 == pid)
			{
				sys::perror("fork");
			}
			else
			if (0 == pid)
			{
				if (-1 == sys::dup2(in[STDIN_FILENO], STDIN_FILENO))
				{
					sys::perror("dup2", in[STDIN_FILENO], STDIN_FILENO);
					return;
				}
				if (-1 == sys::dup2(out[STDOUT_FILENO], STDOUT_FILENO))
				{
					sys::perror("dup2", out[STDOUT_FILENO], STDOUT_FILENO);
					return;
				}

				std::vector<Char*> cmds(args);
				cmds.push_back(nullptr);

				if (-1 == sys::execv(cmds.front(), cmds.data()))
				{
					sys::perror("execv");
				}
				std::exit(EXIT_FAILURE);
			}
			else
			{
				pipe[STDIN_FILENO] = out.release(STDOUT_FILENO);
				pipe[STDOUT_FILENO] = in.release(STDIN_FILENO);
			}
		}
	}
	
	template class impl_pstream<char>;
	template class impl_pstream<wchar_t>;
}

