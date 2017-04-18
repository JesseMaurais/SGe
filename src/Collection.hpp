/** \file
 * Collect objects belonging to a specified class for joint execution of any
 * class methods.
 */

#ifndef Collection_hpp
#define Collection_hpp

#include <set>
#include <cassert>

/// Collection of objects belonging to a specified class Type
template <typename Type> class Collection
{
private:

	typedef Type *Pointer;
	std::set<Pointer> set;

public:

	/// Add object to collection
	bool Add(Pointer that)
	{
		assert(that and "Cannot add null object to collection");
		return set.insert(that).second;
	}

	/// Remove object from collection
	bool Remove(Pointer that)
	{
		assert(that and "Cannot remove null object from collection");
		return set.erase(that) == 1;
	}

	/// True if object is in collection
	bool Has(Pointer that) const
	{
		return set.find(that) != set.end();
	}

	/// True if collection is empty
	bool Empty() const
	{
		return set.empty();
	}

	/// Call the given method with given arguments on all objects in collection
	template <typename ...Args>	void Call(void(Type::*method)(Args...), Args... args)
	{
		for (auto that : set)
		{
			that->method(args...);
		}
	}
	
	/// Call the given constant method with given arguments on all objects in collection
	template <typename ...Args>	void Call(void(Type::*method)(Args...) const, Args... args) const
	{
		for (auto that : set)
		{
			that->method(args...);
		}
	}

	/// Call the given method on all objects and count those return true
	unsigned Count(bool(Type::*method)(void))
	{
		unsigned count = 0;
		for (auto that : set)
		{
			if (that->method())
			{
				++count;
			}
		}
		return count;
	}

	/// Call the given constant method on all objects and count those that return true
	unsigned Count(bool(Type::*method)(void) const) const
	{
		unsigned count = 0;
		for (auto that : set)
		{
			if (that->method())
			{
				++count;
			}
		}
		return count;
	}

	/// Call the given method with given arguments on all objects and count those that return true
	template <typename ...Args>	unsigned Count(bool(Type::*method)(Args...), Args... args)
	{
		unsigned count = 0;
		for (auto that : set)
		{
			if (that->method(args...))
			{
				++count;
			}
		}
		return count;
	}

	/// Call the given constant method with given arguments on all objects and count those that return true
	template <typename ...Args>	unsigned Count(bool(Type::*method)(Args...) const, Args... args) const
	{
		unsigned count = 0;
		for (auto that : set)
		{
			if (that->method(args...))
			{
				++count;
			}
		}
		return count;
	}
};

#endif // file
