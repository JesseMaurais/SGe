#ifndef stl_algorithm_hpp
#define stl_algorithm_hpp

#include <algorithm>

namespace stl
{
	// Non-modifying sequence operations

	template <typename Container, typename Operation>
	inline void for_each(Container &&c, Operation &&op)
	{
		std::for_each(c.begin(), c.end(), op);
	}

	template <typename Value, typename Alloc, template <typename, typename> class Container>
	inline auto find(Container<Value, Alloc> const &c, Value const &value) -> decltype(c.end())
	{
		return std::find(c.begin(), c.end(), value);
	}

	template <typename Container, typename Predicate>
	inline auto find_if(Container &&c, Predicate &&p) -> decltype(c.end())
	{
		return std::find_if(c.begin(), c.end(), p);
	}

	template <typename Container, typename Predicate>
	inline auto find_if_not(Container &&c, Predicate &&p) -> decltype(c.end())
	{
		return std::find_if_not(std::begin(c), std::end(c), p);
	}

	// Modifying sequence operations

	template <typename Container, typename Iterator>
	inline auto copy(Container &&c, Iterator &&it) -> decltype(c.end())
	{
		return std::copy(c.begin(), c.end(), it);
	}

	template <typename Container, typename Operation>
	inline void transform(Container &&c, Operation &&op)
	{
		std::transform(c.begin(), c.end(), c.begin(), op);
	}

	template <typename ContainerA, typename ContainerB, typename Operation>
	inline void transform(ContainerA &&a, ContainerB &&b, Operation &&op)
	{
		std::transform(std::begin(a), std::end(a), std::begin(b), op);
	}

	template <typename Container, typename Generator>
	inline void generate(Container &&c, Generator &&g)
	{
		std::generate(c.begin(), c.end(), g);
	}

	template <typename Container, typename Value>
	inline void replace(Container &&c, Value const &a, Value const &b)
	{
		std::replace(c.begin(), c.end(), a, b);
	}

	// Sorting operations
	
	template <typename Container, typename Compare>
	inline void sort(Container &&c, Compare &&cmp)
	{
		std::sort(c.begin(), c.end(), cmp);
	}
}

#endif // file
