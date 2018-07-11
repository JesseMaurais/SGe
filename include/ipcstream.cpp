#include "ipcstream.hpp"
#include "unisys.hpp"

namespace
{
	pid_t spork(int fd[2])
	{
		if (-1 == pipe(fd))
		{
			return -1;
		}
		pid_t pid = fork();
		if (-1 == pid)
		{
			close(fd[0]);
			close(fd[1]);
		}
		return pid;
	}
}

namespace io
{
	template <class Char, template <class> class Traits>
	basic_iipcstream<Char, Traits>::basic_iipcstream()
	{
		int fd[2];
		pid = spork(fd);
		if (-1 == pid)
		{
			// error
		}
		else
		if (0 == pid)
		{
			if (-1 == dup2(fd[1], STDOUT_FILENO))
			{
				// error
			}
			if (-1 == close(fd[0]))
			{
				// error
			}
		}
		else
		{
			if (-1 == close(fd[1]))
			{
				// error
			}
			base::setfd(fd[0]);
		}
	}

	// Instantiate the templates here
	
	template class basic_iipcstream<char>;
	template class basic_iipcstream<wchar_t>;

	template class basic_oipcstream<char>;
	template class basic_oipcstream<wchar_t>;

	template class basic_ipcstream<char>;
	template class basic_ipcstream<wchar_t>;
}

