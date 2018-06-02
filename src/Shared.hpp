#ifndef Shared_hpp
#define Shared_hpp

#include "Signal.hpp"
#include <memory>

template <typename Type> class Shared : Slot<Type>
{
	using Slot = ::Slot<Type>;
	using Observer = typename Slot::Observer;

	Shared(Type id)
	{
		
	}

private:

	static 
};

#endif // file

