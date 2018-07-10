#ifndef fdbuf_hpp
#define fdbuf_hpp

#include <streambuf>

namespace sys::io
{
	template <class Char, template <class> class Traits = std::char_traits>
	class basic_fdbuf : public std::basic_streambuf<Char, Traits<Char>>
	{
		using base = typename std::basic_streambuf<Char, Traits<Char>>;

	public:

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

		int_type overflow(int_type c) override;
		int_type underflow() override;
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

