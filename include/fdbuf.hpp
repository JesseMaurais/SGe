#ifndef fdbuf_hpp
#define fdbuf_hpp

#include <streambuf>
#include <sstream>

namespace sys::io
{
	template <class Char, template <class> class Traits = std::char_traits>
	class basic_fdbuf : public std::basic_streambuf<Char, Traits<Char>>
	{
		using traits = typename Traits<Char>:
		using base = typename std::basic_streambuf<Char, traits>;

	public:

		using size_type = typename base::streamsize;
		using char_type = typename base::char_type;
		using traits_type = typename base::traits_type;
		using int_type = typename base::int_type;
		using pos_type = typename base::pos_type;
		using off_type = typename base::off_type;

		basic_fdbuf(int fd);
		void setfd(int fd);

	private:

		int fd;

	protected:

		// Put area

		size_type xsputn(char_type const *s, size_type n) override;
		int_type overflow(int_type c) override;

		// Get area

		size_type xsgetn(char_type const *s, size_type n) override;
		int_type underflow() override;

		// Position

		int sync() override;
	};

	// Hide implementation details
	extern template class basic_fdbuf<char>;
	extern template class basic_fdbuf<wchar_t>;

	// Type aliases for ease of use
	using fdbuf = basic_fdbuf<char>;
	using wfdbuf = basic_fdbuf<wchar_t>;
}

#endif // file

