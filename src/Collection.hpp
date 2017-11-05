/** \file
 * Collect objects belonging to a specified class for joint execution of any
 * class methods.
 */

#ifndef Collection_hpp
#define Collection_hpp

#include <set>
#include <cassert>

/// Collection of objects belonging to a specified class Type
template <class Type> class Collection
{
	friend class Type;

private:

	typedef Type *Pointer;
	std::set<Pointer> set;

protected:

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
};

#endif // file
