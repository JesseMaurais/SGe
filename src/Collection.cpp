#include "Collection.hpp"
#include "SDL.hpp"

CollectionInterface::CollectionInterface()
{
	SDL_verify(Collections::Instance().Add(this));
}

CollectionInterface::~CollectionInterface()
{
	SDL_verify(Collections::Instance().Remove(this));
}
