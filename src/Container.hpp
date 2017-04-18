/** \file
 * Singleton of object collections used to update the resources of objects
 * after state changes.
 */

#ifndef Container_hpp
#define Container_hpp

#include "Collection.hpp"

/// Basic interface used for Containers
class ContainerInterface
{
public:
	ContainerInterface();
	virtual ~ContainerInterface();
};

/// Collection of objects belonging to a specified class Type
template <typename Type> class Container final
	: ContainerInterface, public Collection<Type>
{
private:

	Container() = default;

	~Container()
	{
		assert(Empty() and "Some objects were not deleted");
	}

public:

	/// Access the singleton
	static Container &Instance()
	{
		static Container singleton;
		return singleton;
	}
};

/// Container of all other containers
using Containers = Container<ContainerInterface>;
template <> Containers::Container();

#endif // file
