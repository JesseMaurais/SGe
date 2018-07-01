#ifndef format_hpp
#define format_hpp

#include <locale>
#include <string>
#include <sstream>
#include <string_view>
#include "algorithm.hpp"

namespace fmt
{
	template <typename T>
	inline std::string to_string(T const &value)
	{
		return std::to_string(value);
	}

	template <>
	inline std::string to_string(std::string const &value)
	{
		return value;
	}

	template <>
	inline std::string to_string(std::string_view const &value)
	{
		return std::string(value.data(), value.size());
	}

	template <>
	inline std::string to_string(char const * const &value)
	{
		return std::string(value, std::strlen(value));
	}

	template <>
	inline std::string to_string(char * const &value)
	{
		return std::string(value, std::strlen(value));
	}

	template <>
	inline std::string to_string(char const & value)
	{
		return std::string(1, value);
	}

	// Basic string formatting tools

	inline void replace(std::string &buffer, std::string const &s, std::string const &r)
	{
		auto const length = s.length();
		constexpr auto end = std::string::npos;
		for (auto at = buffer.find(s); at != end; at = buffer.find(s, at+length))
		{
			buffer.replace(at, length, r);
		}
	}

	class format
	{
	public:

		format(std::string_view s, std::string_view begin_tag="{", std::string_view end_tag="}")
			: buffer(to_string(s))
			, begin(begin_tag)
			, end(end_tag)
			, index(0)
		{}

		template <typename T> format& operator % (T&& arg)
		{
			replace(buffer, next_tag(), to_string(arg));
			return *this;
		}

		operator std::string_view() const
		{
			return buffer;
		}

		operator std::string()
		{
			return buffer;
		}

	private:

		std::string buffer, begin, end;
		std::string::size_type index;

		std::string next_tag()
		{
			std::stringstream tag;
			tag << begin << ++index << end;
			return tag.str();
		}
	};

	inline void split(std::vector<std::string> &tokens, std::string const &string, std::string const &del)
	{
		using size_type = std::string::size_type;
		constexpr auto end = std::string::npos;
		auto const length = del.length();
		for (size_type next = string.find(del), last = 0; last != end; next = string.find(del, last))
		{
			tokens.emplace_back(string.substr(last, next-last));
			last = (next == end) ? next : (next + length);
		}
	}

	template <typename Container>
	inline std::string join(Container const &tokens, std::string const &delimiter)
	{
		std::stringstream stream;
		auto it = std::ostream_iterator<std::string>(stream, delimiter.c_str());
		std::copy(std::begin(tokens), std::end(tokens), it);
		return stream.str();
	}

	inline std::string to_upper(std::string string)
	{
		constexpr auto toupper = [](char c) { return std::toupper(c, std::locale()); };
		algo::transform(string, toupper);
		return string;
	}

	inline std::string to_lower(std::string string)
	{
		constexpr auto tolower = [](char c) { return std::tolower(c, std::locale()); };
		algo::transform(string, tolower);
		return string;
	}

	inline std::string::iterator trim_begin(std::string &string)
	{
		constexpr auto isblank = [](char c) { return std::isblank(c, std::locale()); };
		return string.erase(string.begin(), algo::find_if(string, isblank));
	}

	inline std::string::iterator trim_end(std::string &string)
	{
		constexpr auto isblank = [](char c) { return std::isblank(c, std::locale()); };
		return string.erase(algo::find_if_not(string, isblank), string.end());
	}

	inline bool trim(std::string &string)
	{
		return trim_begin(string) != trim_end(string);
	}

	inline std::string quote(std::string_view string)
	{
		return format("\"{1}\"") % string;
	}

	inline std::string key_value(std::string const &key, std::string const &value)
	{
		return format("{1}={2}") % key % value;
	}

	inline std::pair<std::string_view, std::string_view> key_value(std::string const &string)
	{
		std::vector<std::string> pair;
		split(pair, string, "=");
		if (pair.size() == 2)
		{
			return std::pair(pair.front(), pair.back());
		}
		return std::pair("", "");
	}

	inline bool find(std::string_view string, std::string_view what)
	{
		return string.find(what) != std::string_view::npos;
	}
}

#endif // file
