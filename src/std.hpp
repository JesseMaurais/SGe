#ifndef STL_hpp
#define STL_hpp

#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <cassert>

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

	template  <typename Src>
	std::string to_string(const Src &val)
	{
		return std::to_string(val);
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

	template <typename Container, typename Predicate>
	auto count_if(Container& c, Predicate& p)
	{
		return std::count_if(c.begin(), c.end(), p);
	}
}

namespace str
{
	inline std::string::size_type replace(std::string &string, std::string const &search, std::string const &replace)
	{
		std::string::size_type index, count = 0;
		std::string::size_type const length = search.length();
		for (index = string.find(search); std::string::npos != index; index = string.find(search, index + length))
		{
			string.replace(index, length, replace);
			++count;
		}
		return count;
	}

	class format
	{
	private:

		std::string string;
		std::size_t index;
		std::size_t subs;

	public:

		format(std::string const &s)
			: string(s)
			, index(0)
			, subs(0)
		{}

		template <typename Type> format &operator%(Type const &arg)
		{
			using std::to_string;
			std::string const search = std::string('%') + to_string(++index);
			std::string const replace = to_string(arg);
			subs += str::replace(string, search, replace);
			return *this;
		}

		operator std::string()
		{
			return string;
		}

		operator std::size_t()
		{
			return subs;
		}
	};
}

namespace io
{
	template <typename... Args>
	std::size_t sprintf(std::string &string, std::string const &format, Args... args)
	{
		return (str::format(string = format) ... % args);
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
