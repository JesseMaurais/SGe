#ifndef pipe_hpp
#define pipe_hpp

namespace sys::io
{
	constexpr int NFD = -1;

	class auto_fd
	{
	public:

		~auto_fd();

		auto_fd(int fd = NFD)
		{
			this->fd = fd;
		}

		operator int() const
		{
			return fd;
		}

		operator bool() const
		{
			return NFD != fd;
		}

		int release()
		{
			int fd = this->fd;
			this->fd = NFD;
			return fd;
		}

	private:

		int fd;
	};

	class auto_pipe
	{
	public:

		auto_pipe();

		int operator[](std::size_t id) const
		{
			return id < 2 ? fds[id] : NFD;
		}

		operator bool() const
		{
			return fds[0] or fds[1];
		}

		int release(std::size_t id)
		{
			return id < 2 ? fds[id].release() : NFD;
		}

	private:

		auto_fd fds[2];
	};
}

#endif // file
