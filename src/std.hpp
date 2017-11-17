#ifndef stdcpp_hpp
#define stdcpp_hpp

#include "stl/algorithm.hpp"
#include "stl/string.hpp"

#include <iostream>
#include <limits>
#include <type_traits>
#include <string>
#include <vector>
#include <cassert>
#include <cctype>

#ifndef NDEBUG
#define verify(condition) assert(condition)
#else
#define verify(condtiion) (condition)
#endif

namespace
{
	template <typename Dst, typename Src>
	Dst to(const Src &value)
	{
		using dst = std::numeric_limits<Dst>;
		using src = std::numeric_limits<Src>;
		static_assert(dst::is_exact == src::is_exact, "Exact mismatch");
		constexpr auto min = dst::is_signed and src::is_signed ? dst::min() : 0;
		constexpr auto max = dst::max();
		assert(min <= value);
		assert(value <= max);
		return static_cast<Dst>(value);
	}

	template <typename Src>
	unsigned to_unsigned(const Src &value)
	{
		using source = std::numeric_limits<Src>;
		static_assert(source::is_integer, "Value must be an integer type");
		return to<unsigned>(value);
	}

	template <typename Src>
	int to_int(const Src &val)
	{
		using source = std::numeric_limits<Src>;
		static_assert(source::is_integer, "Value must be an integer type");
		return to<int>(val);
	}
}

namespace stl
{
	template <typename Type>
	void append(std::vector<Type> &to, std::vector<Type> const &from)
	{
		auto const size_from = from.size();
		auto const size_to = to.size();
		to.reserve(size_from + size_to);
		to.insert(to.end(), from.begin(), from.end());
	}

	template <typename Container>
	Container &max(Container &a, Container &b)
	{
		return a.size() > b.size() ? a : b;
	}

	template <typename Container>
	Container &min(Container &a, Container &b)
	{
		return a.size() < b.size() ? a : b;
	}

	template <typename Container>
	std::pair<Container&, Container&> minmax(Container &a, Container &b)
	{
		return std::make_pair(stl::min(a, b), stl::max(a, b));
	}

	// Basic string formatting tools

	inline void replace(std::string &string, std::string const &search, std::string const &replace)
	{
		std::string::size_type const length = search.length();
		for (std::string::size_type pos = string.find(search); std::string::npos != pos; pos = string.find(search, pos + length))
		{
			string.replace(pos, length, replace);
		}
	}

	template <template <typename, typename> class Container>
	void split(Container<std::string, std::allocator<std::string>> &tokens, std::string const &string, std::string const &delimiter)
	{
		std::string::size_type const length = delimiter.length();
		for (std::string::size_type next = string.find(delimiter), last = 0; std::string::npos != last; next = string.find(delimiter, last))
		{
			tokens.emplace_back(string.substr(last, next - last));
			last = std::string::npos == next ? next : next + length;
		}
	}

	template <template <typename, typename> class Container>
	std::string merge(Container<std::string, std::allocator<std::string>> const &tokens, std::string const &delimiter)
	{
		std::stringstream stream;
		auto it = std::ostream_iterator<std::string>(stream, delimiter.c_str());
		std::copy(tokens.begin(), tokens.end(), it);
		return stream.str();
	}

	inline std::string to_upper(std::string string)
	{
		constexpr auto upper = [](char c) { return std::toupper(c); };
		stl::transform(string, upper);
		return string;
	}

	inline std::string to_lower(std::string string)
	{
		constexpr auto lower = [](char c) { return std::tolower(c); };
		stl::transform(string, lower);
		return string;
	}

	inline std::string::iterator trim_begin(std::string &string)
	{
		constexpr auto is_blank = [](char c) { return not std::isblank(c); };
		return string.erase(string.begin(), stl::find_if(string, is_blank));
	}

	inline std::string::iterator trim_end(std::string &string)
	{
		constexpr auto is_blank = [](char c) { return not std::isblank(c); };
		return string.erase(stl::find_if(string, is_blank), string.end());
	}

	inline bool trim(std::string &string)
	{
		return trim_begin(string) != trim_end(string);
	}

	// Type-safe printf-like string formatting tools

	class format
	{
	public:

		format(std::string const &s)
			: string(s)
			, index(0)
		{}

		template <typename Type> format &operator%(Type const &arg)
		{
			std::string const search = std::string("%") + std::to_string(++index);
			std::string const replace = to_string(arg);
			stl::replace(string, search, replace);
			return *this;
		}

		operator std::string()
		{
			return string;
		}

	private:

		std::string string;
		std::string::size_type index;
	};

	template <typename... Args>
	std::size_t sprintf(std::string &string, std::string const &format, Args... args)
	{
		string = (stl::format(format) % ... % args);
		return sizeof...(args);
	}

	template <typename... Args>
	std::size_t fprintf(std::ostream &stream, std::string const &format, Args... args)
	{
		std::string string;
		std::size_t const subs = sprintf(string, format, args...);
		stream << string;
		return subs;
	}

	template <typename... Args>
	std::size_t printf(std::string const &format, Args... args)
	{
		return fprintf(std::cout, format, args...);
	}

	inline std::string quote(std::string const &string)
	{
		std::string result;
		sprintf(result, "\"%1\"", string);
		return result;
	}

	inline std::string param_value(std::string const &param, std::string const &value)
	{
		std::string result;
		sprintf(result, "%1=%2", param, value);
		return result;
	}
}

#endif // file
