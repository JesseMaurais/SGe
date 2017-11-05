#include "Container.hpp"
#include "std.hpp"

ContainerInterface::ContainerInterface()
{
	verify(Containers::Instance().Add(this));
}

ContainerInterface::~ContainerInterface()
{
	verify(Containers::Instance().Remove(this));
}

template <> Containers::Container()
{
	// Specialize so we do not reenter Containers::Instance
	verify(Add(this));
}
