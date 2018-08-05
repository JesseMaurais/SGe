#ifndef num_hpp
#define num_hpp

#include <stdexcept>
#include <charconv>
#include <cassert>
#include <limits>
#include <string>
#include "io.hpp"

namespace
{
	template <typename Type> constexpr bool is_signed = std::is_signed<Type>::value;

	template <typename Min, typename Max> std::overflow_error overflow(Min min, Max max)
	{
		std::string string;
		io::sprintf(string, "max {1} < {2}", min, max);
		return std::overflow_error(string);
	}

	template <typename Min, typename Max> std::underflow_error underflow(Min min, Max max)
	{
		std::string string;
		io::sprintf(string, "{1} < {2} min", min, max);
		return std::underflow_error(string);
	}

	template <typename Target, typename Source> Target to(Source value)
	{
		if constexpr (is_signed<Target> == is_signed<Source>)
		{
			constexpr auto target_min = std::numeric_limits<Target>::lowest();
			constexpr auto source_min = std::numeric_limits<Source>::lowest();

			if constexpr (target_min > source_min)
			{
				if (value < target_min) throw underflow(target_min, value);
			}

			constexpr auto target_max = std::numeric_limits<Target>::max();
			constexpr auto source_max = std::numeric_limits<Source>::max();

			if constexpr (target_max < source_max)
			{
				if (target_max < value) throw overflow(value, target_max);
			}
		}
		else
		{
			if constexpr (is_signed<Source>)
			{
				if (value < 0) throw underflow(value, 0);

				using Unsign = typename std::make_unsigned<Source>::type;
				constexpr auto unsign_max = std::numeric_limits<Unsign>::max();
				constexpr auto target_max = std::numeric_limits<Target>::max();

				if constexpr (target_max < unsign_max)
				{
					if (target_max < Unsign(value)) throw overflow(target_max, value);
				}
			}
			if constexpr (is_signed<Target>)
			{
				using Signed = typename std::make_signed<Source>::type;
				constexpr auto signed_max = std::numeric_limits<Signed>::max();
				constexpr auto target_max = std::numeric_limits<Target>::max();

				if constexpr (target_max < signed_max)
				{
					if (target_max < Signed(value)) throw overflow(target_max, value);
				}
			}
		}
		return static_cast<Target>(value);
	}

	template <typename Target> Target to(std::string_view string)
	{
		Target value;
		auto const result = std::from_chars(string.data(), string.data() + string.size(), value);
		if (std::make_error_code(result.ec)) throw std::bad_cast();
		return value;
	}

	template <typename Source> unsigned to_unsigned(Source value)
	{
		return to<unsigned>(value);
	}

	template <typename Source> int to_int(Source value)
	{
		return to<int>(value);
	}
}

#endif // file
