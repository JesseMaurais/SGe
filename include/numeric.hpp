#ifndef stl_numeric_hpp
#define stl_numeric_hpp

#include <cassert>
#include <limits>

namespace
{
	template <typename Target, typename Source> Target to(Source const &value)
	{
		using target = std::numeric_limits<Target>;
		constexpr auto target_min = target::lowest();
		constexpr auto target_max = target::max();

		using source = std::numeric_limits<Source>;
		constexpr auto source_min = source::lowest();
		constexpr auto source_max = source::max();

		if constexpr (target_min > source_min)
		{
			if (value < target_min) throw std::out_of_range();
		}
		if constexpr (target_max < source_max)
		{
			if (target_max < value) throw std::out_of_range();
		}

		return static_cast<Target>(value);
	}

	template <typename Source> unsigned to_unsigned(Source const &value)
	{
		return to<unsigned>(value);
	}

	template <typename Source> int to_int(Source const &value)
	{
		return to<int>(value);
	}
}

#endif // file
