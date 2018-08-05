#include "auto.hpp"
#include "unisys.hpp"
#include <cstdio>

namespace sys::io
{
	auto_fd::~auto_fd()
	{
		if (NFD == fd)
		{
			return;
		}
		if (::close(fd))
		{
			std::perror(__FUNCTION__);
		}
	}

	auto_pipe::auto_pipe()
	{
		int fds[2];
		if (::pipe(fds))
		{
			std::perror(__FUNCTION__);
			return;
		}
		this->fds[0] = fds[0];
		this->fds[1] = fds[1];
	}
}

