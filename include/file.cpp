#include "file.hpp"
#include "sys.hpp"
#include "err.hpp"

namespace sys::file
{
	descriptor::~descriptor()
	{
		if (-1 == fd)
		{
			return;
		}
		if (sys::close(fd))
		{
			sys::perror("close", fd);
		}
	}

	pipe::pipe()
	{
		int fds[2];
		if (sys::pipe(fds))
		{
			sys::perror("pipe");
			return;
		}
		this->fds[0] = fds[0];
		this->fds[1] = fds[1];
	}
}

