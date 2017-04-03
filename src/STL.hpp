#ifndef STL_hpp
#define STL_hpp

#include <limits>
#include <cassert>

namespace stl
{
	template <typename Dst, typename Src> inline Dst as(const Src &value)
	{
		using limits = std::numeric_limits<Dst>;
		using source = std::numeric_limits<Src>;
		static_assert(limits::is_exact == source::is_exact, "Exact mismatch");
		//assert(limits::min() < value);
		//assert(value < limits::max());
		return static_cast<Dst>(value);
	}

	template <typename Src> inline unsigned as_unsigned(const Src &value)
	{
		using source = std::numeric_limits<Src>;
		static_assert(source::is_integer, "Value must be an integer type");
		return as<unsigned>(value);
	}

	template <typename Src> inline int as_int(const Src &val)
	{
		using source = std::numeric_limits<Src>;
		static_assert(source::is_integer, "Value must be an integer type");
		return as<int>(val);
	}

	template <typename Type> inline void merge(std::vector<Type> const &from, std::vector<Type> &to)
	{
		std::size_t from_size = from.size();
		std::size_t to_size = from.size();
		to.reserve(from_size + to_size);
		to.insert(to.end(), from.begin(), from.end());
	}
}

#endif // file
