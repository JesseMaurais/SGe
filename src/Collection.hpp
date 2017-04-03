/** \file
 * Collect objects belonging to a specified class for joint execution of any
 * class methods. Used to update the resources of objects after state changes.
 */

#ifndef Collection_hpp
#define Collection_hpp

#include <set>
#include "SDL.hpp"

/// Basic interface used for CollectionManager
class CollectionInterface
{
public:
	CollectionInterface();
	virtual ~CollectionInterface();
};

/// Collection of objects belonging to a specified class Type
template <typename Type> class Collection final : CollectionInterface
{
private:

	std::set<Type*> set;

	Collection() = default;

	~Collection()
	{
		SDL_assert(set.empty() and "Some objects were not deleted");
	}

public:

	/// Access the singleton
	static Collection &Instance()
	{
		static Collection singleton;
		return singleton;
	}

	/// Add object to collection
	bool Add(Type *object)
	{
		SDL_assert(object and "Cannot add null object to collection");
		return set.insert(object).second;
	}

	// Remove object from collection
	bool Remove(Type *object)
	{
		SDL_assert(object and "Cannot remove null object from collection");
		return set.erase(object) == 1;
	}

	/// True if object is in collection
	bool Has(Type *object)
	{
		return set.find(object) != set.end();
	}

	/// Call the given method with given arguments on all objects in collection
	template <typename ...Args>	void Call(void(Type::*method)(Args...), Args... args)
	{
		for (auto item : set)
		{
			item->method(args...);
		}
	}
	
	/// Call the given method with given arguments on all objects and count those returning true
	template <typename ...Args>	unsigned Count(bool(Type::*method)(Args...), Args... args)
	{
		unsigned count = 0;
		for (auto item : set)
		{
			if (item->method(args...))
			{
				++count;
			}
		}
		return count;
	}
};

/// Collection of all other collections
using Collections = Collection<CollectionInterface>;

template <> Collections::Collection()
{
	// Specialize so we do not reenter Collections::Instance
	SDL_verify(Add(this));
}

#endif // file
