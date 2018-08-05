#include "fd.hpp"
#include "fmt.hpp"
#include "unisys.hpp"
#include <cstdio>

namespace
{
	using namespace std::ios_base;
	int convert(openmode mode)
	{
		int flags = 0;
		if (mode & in and mode & out)
		{
			flags |= O_RDWR;
		}
		else if (mode & out)
		{
			flags |= O_WRONLY;
		}
		else if (mode & in)
		{
			flags |= O_RDONLY;
		}
		if (mode & app)
		{
			flags |= O_APPEND;
		}
		if (mode & trunc)
		{
			flags |= O_TRUNC;
		}
		if constexpr (sys::linux)
		{
			flags |= O_DIRECT;
		}
	}
}

namespace sys::io
{
	auto_fd::auto_fd(std::string_view view, std::ios_base::openmode mode)
	{
		std::string const path = fmt::to_string(view);
		fd = ::open(path.c_str(), convert(mode));
		if (NFD == fd)
		{
			std::perror("open");
			return;
		}
	}

	auto_fd::~auto_fd()
	{
		if (NFD == fd)
		{
			return;
		}
		if (::close(fd))
		{
			std::perror("close");
		}
	}

	auto_pipe::auto_pipe()
	{
		int fds[2];
		if (::pipe(fds))
		{
			std::perror("pipe");
			return;
		}
		this->fds[0] = fds[0];
		this->fds[1] = fds[1];
	}
}

