#ifndef alg_hpp
#define alg_hpp

#include <algorithm>

namespace stl
{
	// Non-modifying sequence operations

	template <typename Container, typename Operation>
	inline void for_each(Container &&c, Operation &&op)
	{
		std::for_each(begin(c), end(c), op);
	}

	template <typename Value, typename Alloc, template <typename, typename> class Container>
	inline auto find(Container<Value, Alloc> const &c, Value const &value) -> decltype(c.end())
	{
		return std::find(begin(c), end(c), value);
	}

	template <typename Container, typename Predicate>
	inline auto find_if(Container &&c, Predicate &&p) -> decltype(c.end())
	{
		return std::find_if(begin(c), end(c), p);
	}

	template <typename Container, typename Predicate>
	inline auto find_if_not(Container &&c, Predicate &&p) -> decltype(c.end())
	{
		return std::find_if_not(begin(c), end(c), p);
	}

	// Modifying sequence operations

	template <typename Container, typename Iterator>
	inline auto copy(Container &&c, Iterator &&it) -> decltype(c.end())
	{
		return std::copy(begin(c), end(c), it);
	}

	template <typename Container, typename Operation>
	inline void transform(Container &&c, Operation &&op)
	{
		std::transform(begin(c), end(c), begin(c), op);
	}

	template <typename ContainerA, typename ContainerB, typename Operation>
	inline void transform(ContainerA &&a, ContainerB &&b, Operation &&op)
	{
		std::transform(begin(a), end(a), begin(b), op);
	}

	template <typename Container, typename Generator>
	inline void generate(Container &&c, Generator &&g)
	{
		std::generate(begin(c), end(c), g);
	}

	template <typename Container, typename Value>
	inline void replace(Container &&c, Value const &a, Value const &b)
	{
		std::replace(begin(c), end(c), a, b);
	}

	template <typename ToContainer, typename FromContainer>
	inline void append(ToContainer& to, FromContainer const &from)
	{
		to.reserve(to.size() + from.size());
		to.insert(end(to), begin(from), end(from));
	}

	// Sorting operations
	
	template <typename Container, typename Compare>
	inline void sort(Container &&c, Compare &&cmp)
	{
		std::sort(begin(c), end(c), cmp);
	}
}

#endif // file
