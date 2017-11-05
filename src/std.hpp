#ifndef stdcpp_hpp
#define stdcpp_hpp

#include <algorithm>
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
#define verify(condtiion) condition
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

	template <typename Src>
	std::string to_string(const Src &val)
	{
		return std::to_string(val);
	}

	inline std::string to_string(const unsigned char *val)
	{
		return std::string(reinterpret_cast<const char*>(val));
	}

	inline std::string to_string(const char *val)
	{
		return std::string(val);
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
}

namespace str
{
	inline void replace(std::string &string, std::string const &search, std::string const &replace)
	{
		std::string::size_type const length = search.length();
		for (auto pos = string.find(search); std::string::npos != pos; pos = string.find(search, pos))
		{
			string.replace(pos, length, replace);
			pos += length;
		}
	}

	inline void split(std::vector<std::string> &tokens, std::string const &string, std::string const &delimiter)
	{
		constexpr std::string::size_type start = 0;
		std::string::size_type const length = delimiter.length();
		for (auto next = string.find(delimiter), last = start; std::string::npos != next; next = string.find(delimiter, last))
		{
			bool end = std::string::npos == next;
			auto count = end ? next : next - last;
			tokens.emplace_back(string.substr(last, count));
			last = end ? next : next + length;
		}
	}

	inline std::string to_upper(std::string string)
	{
		constexpr auto upper = [](unsigned char c) { return std::toupper(c); };
		std::transform(string.begin(), string.end(), string.begin(), upper);
		return string;
	}

	inline std::string to_lower(std::string string)
	{
		constexpr auto lower = [](unsigned char c) { return std::tolower(c); };
		std::transform(string.begin(), string.end(), string.begin(), lower);
		return string;
	}

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
			str::replace(string, search, replace);
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
}

namespace io
{
	template <typename... Args>
	std::size_t sprintf(std::string &string, std::string const &format, Args... args)
	{
		auto output = (str::format(format) % ... % args);
		string = output;
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
}

#endif // file
