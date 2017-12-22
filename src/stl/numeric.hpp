#ifndef stl_numeric_hpp
#define stl_numeric_hpp

#include <numeric>

namespace stl
{
	template <typename Container, typename Value>
	void iota(Container&& c, Value&& value = 0)
	{
		std::iota(std::begin(c), std::end(c), value);
	}
}

#endif // file
