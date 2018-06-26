#ifndef stl_format_hpp
#define stl_format_hpp

#include <string>
#include <sstream>
#include "algorithm.hpp"

namespace stl
{
	template <class Char> class format
	{
	public:

		using char_type = std::ctype<Char>;
		using string = std::basic_string<Char>;
		using view = std::basic_string_view<Char>;
		using stream = std::basic_stringstream<Char>;
		using size_type = string::size_type;

		format(string const &s)
			: buffer(s)
			, index(0)
			, ctype()
		{}

		template <typename Type> format& operator%(Type const &arg)
		{
			string const search = to_tag(++index);
			string const replace = to_string(arg);
			stl::replace(string, search, replace);
			return *this;
		}

		operator & string() const
		{
			return buffer;
		}

	private:

		char_type ctype;
		string buffer;
		size_type index;

		static string to_tag(size_type n)
		{
			stream tag;
			constexpr auto delimiter = Char('%');
			tag << delimiter << n << delimiter;
			return tag.str();
		}
	};


	// Basic string formatting tools

	template <typename Char>
	void replace
	(
	 std::basic_string<Char> &string,
	 std::basic_string<Char> const &search,
	 std::basic_string<Char> const &replace
	)
	{
		using size_type = std::basic_string<Char>::size_type;
		constexpr auto npos = std::basic_string<Char>::npos;
		auto const length = search.length();
		for (size_type pos = string.find(search); npos != pos; pos = string.find(search, pos+length))
		{
			string.replace(pos, length, replace);
		}
	}

	template <typename Char, typename Container>
	void split
	(
	 Container &tokens,
	 std::basic_string<Char> const &string,
	 std::basic_string<Char> const &delimiter
	)
	{
		using size_type = std::basic_string<Char>::size_type;
		constexpr auto npos = std::basic_string<Char>::npos;
		auto const length = delimiter.length();
		for (size_type next = string.find(delimiter), last = 0; npos != last; next = string.find(delimiter, last))
		{
			tokens.emplace_back(string.substr(last, next - last));
			last = npos == next ? next : next + length;
		}
	}

	template <typename Char, typename Container>
	std::basic_string<Char> merge
	(
	 Container const &tokens,
	 std::basic_string<Char> const &delimiter
	)
	{
		std::basic_stringstream<Char> stream;
		auto it = std::ostream_iterator<std::basic_string<Char>>(stream, delimiter.c_str());
		stl::copy(tokens, it);
		return stream.str();
	}

	template <typename Char>
	std::basic_string<Char> to_upper(std::basic_string<Char> string)
	{
		stl::transform(string, stl::to_upper);
		return string;
	}

	template <typename Char>
	std::basic_string<Char> to_lower(std::basic_string<Char> string)
	{
		stl::transform(string, stl::to_lower);
		return string;
	}

	template <typename Char>
	std::basic_string<Char>::iterator trim_begin(std::basic_string<Char> &string)
	{
		return string.erase(string.begin(), stl::find_if(string, stl::is_blank));
	}

	template <typename Char>
	std::basic_string<Char>::iterator trim_end(std::basic_string<Char> &string)
	{
		return string.erase(stl::find_if_not(string, stl::is_blank), string.end());
	}

	template <typename Char>
	bool trim(std::basic_string<Char> &string)
	{
		return trim_begin(string) != trim_end(string);
	}

	template <typename Char>
	std::basic_string<Char> quote(std::basic_string<Char> const &string)
	{
		std::string result;
		sprintf(result, "\"%1%\"", string);
		return result;
	}
}

#endif // file
