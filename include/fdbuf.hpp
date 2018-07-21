#ifndef fdbuf_hpp
#define fdbuf_hpp

#include <streambuf>

namespace sys::io
{
	template
	<
	 class Char,
	 template <class> class Traits = std::char_traits
	>
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
		base *setbuf(char_type* s, size_type n) override;
		base *setbuf(char_type* s, size_type n, size_type m);

	private:

		int fd;

	protected:

		// put area
		size_type xsputn(char_type const *s, size_type n) override;
		int_type overflow(int_type c) override;

		// get area
		size_type xsgetn(char_type const *s, size_type n) override;
		int_type underflow() override;

		// position
		int sync() override;
	};

	// implementations
	extern template class basic_fdbuf<char>;
	extern template class basic_fdbuf<wchar_t>;

	// aliases
	using fdbuf = basic_fdbuf<char>;
	using wfdbuf = basic_fdbuf<wchar_t>;
}

#endif // file

