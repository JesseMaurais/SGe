#include "Container.hpp"
#include "SDL.hpp"

ContainerInterface::ContainerInterface()
{
	SDL_verify(Containers::Instance().Add(this));
}

ContainerInterface::~ContainerInterface()
{
	SDL_verify(Containers::Instance().Remove(this));
}

template <> Containers::Container()
{
	// Specialize so we do not reenter Containers::Instance
	SDL_verify(Add(this));
}
