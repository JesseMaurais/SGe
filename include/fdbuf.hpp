#ifndef fdbuf_hpp
#define fdbuf_hpp

#include <streambuf>

namespace sys::io
{
	class fdbuf : public std::streambuf
	{
	public:

		fdbuf(int fd);

	private:

		int fd;
		char inbuf[BUFSIZ];
		char outbuf[BUFSIZ];

	protected:

		int_type overflow(int_type c = traits_type::eof()) override;
		int_type underflow() override;
		int_type sync() override;
	};
};

#endif // file

