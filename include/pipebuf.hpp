#ifndef pipebuf_hpp
#define pipebuf_hpp

#include "fdbuf.hpp"

namespace sys::io
{
	template
	<
	 class Char,
	 template <class> class Traits = std::char_traits
	>
	class basic_pipebuf : public basic_iobuf<Char, Traits>
	{
		using base = basic_iobuf<Char, Traits>;
		using size_type = typename base::streamsize;
		using char_type = typename base::char_type;
		using fdbuf = basic_fdbuf<Char, Traits>;

	public:

		basic_pipbuf(int fds[2])
		{
			setfds(fds);
		}

		void setfds(int fds[2])
		{
			this->fds[0].setfd(fd[0]);
			this->fds[1].setfd(fd[1]); 
		}

	private:

		fdbuf fds[2];

	protected:

		size_type xsputn(char_type *s, size_type n) override
		{
			return fds[1].xsputn(s, n);
		}

		size_type xsgetn(char_type *s, size_type n) override
		{
			return fds[0].xsgetn(s, n);
		}
	};

	using pipebuf = basic_pipebuf<char>;
	using wpipebuf = basic_pipebuf<wchar_t>;

	extern template class basic_pipebuf<char>;
	extern template class basic_pipebuf<wchar_t>;
}

#endif // file
