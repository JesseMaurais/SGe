#ifndef stl_algorithm_hpp
#define stl_algorithm_hpp

#include <algorithm>

namespace stl
{
	template <typename Container, typename Operation>
	inline void for_each(Container &&c, Operation &&op)
	{
		std::for_each(c.begin(), c.end(), op);
	}

	template <typename Container, typename Predicate>
	inline auto find_if(Container &&c, Predicate &&p) -> decltype(c.begin())
	{
		return std::find_if(c.begin(), c.end(), p);
	}

	template <typename Container, typename Operation>
	inline void transform(Container &&c, Operation &&op)
	{
		std::transform(c.begin(), c.end(), c.begin(), op);
	}

	template <typename Container, typename Generator>
	inline void generate(Container &&c, Generator &&g)
	{
		std::generate(c.begin(), c.end(), g);
	}
}

#endif // file
