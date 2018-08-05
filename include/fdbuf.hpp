#ifndef fdbuf_hpp
#define fdbuf_hpp

#include "iobuf.hpp"

namespace sys::io
{
	template
	<
	 class Char,
	 template <class> class Traits = std::char_traits
	>
	class basic_fdbuf : public basic_iobuf<Char, Traits>
	{
		using base = basic_iobuf<Char, Traits>;
		using size_type = typename base::streamsize;
		using char_type = typename base::char_type;

	public:

		basic_fdbuf(int fd)
		{
			setfd(fd);
		}

		void setfd(int fd)
		{
			this->fd = fd;
		}

	private:

		int fd;

	protected:

		size_type xsputn(char_type *s, size_type n) override;
		size_type xsgetn(char_type *s, size_type n) override;
	};

	using fdbuf = basic_fdbuf<char>;
	using wfdbuf = basic_fdbuf<wchar_t>;

	extern template class basic_fdbuf<char>;
	extern template class basic_fdbuf<wchar_t>;
}

#endif // file

