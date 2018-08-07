#include "fd.hpp"
#include "os.hpp"
#include "fmt.hpp"
#include "sys.hpp"
#include "err.hpp"
#include "ios.hpp"
#include <cstdio>

namespace
{
	int convert(io::openmode mode)
	{
		using namespace io;
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
		if (mode & bin)
		{
			flags |= O_BINARY;
		}
		if (mode & app)
		{
			flags |= O_APPEND;
		}
		if (mode & trunc)
		{
			flags |= O_TRUNC;
		}
		if constexpr (sys::LINUX)
		{
			flags |= O_DIRECT;
		}
		return flags;
	}
}

namespace sys::io
{
	auto_fd::auto_fd(std::string_view view, ::io::openmode mode)
	{
		std::string const path = fmt::to_string(view);
		fd = sys::open(path.c_str(), convert(mode));
		if (NFD == fd)
		{
			sys::ferror("open", path, mode);
			return;
		}
	}

	auto_fd::~auto_fd()
	{
		if (NFD == fd)
		{
			return;
		}
		if (sys::close(fd))
		{
			sys::ferror("close", fd);
		}
	}

	auto_pipe::auto_pipe()
	{
		int fds[2];
		if (sys::pipe(fds))
		{
			sys::ferror("pipe");
			return;
		}
		this->fds[0] = fds[0];
		this->fds[1] = fds[1];
	}
}

