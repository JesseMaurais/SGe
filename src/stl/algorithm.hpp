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

	template <typename Container, typename Predicate>
	inline auto find_if_not(Container &&c, Predicate &&p) -> decltype(c.begin())
	{
		return std::find_if_not(std::begin(c), std::end(c), p);
	}

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

	template <typename Container, typename Value>
	inline auto lower_bound(Container&& c, Value const &a) -> decltype(c.end())
	{
		return lower_bound(c.begin(), c.end(), a);
	}

	template <typename Container, typename Value, typename Compare>
	inline auto lower_bound(Container&& c, Value const &a, Compare&& op) -> decltype(c.end())
	{
		return lower_bound(c.begin(), c.end(), a, op);
	}

	template <typename Container, typename Value>
	inline auto upper_bound(Container&& c, Value const &a) -> decltype(c.end())
	{
		return upper_bound(c.begin(), c.end(), a);
	}

	template <typename Container, typename Value, typename Compare>
	inline auto upper_bound(Container&& c, Value const &a, Compare&& op) -> decltype(c.end())
	{
		return upper_bound(c.begin(), c.end(), a, op);
	}
}

#endif // file
