#include "Manager.hpp"

#include "stl.hpp"
#include "SDL.hpp"

unsigned Manager::Add(Source *that)
{
	unsigned index;
	auto const size = sources.size();
	for (index = 0; index < size; ++index)
	{
		if (not sources[index])
		{
			return index;
		}
	}
	SDL_assert(size == index);
	sources.push_back(that);
	return index;
}

Source *Manager::Remove(unsigned index)
{
	Source *that = sources.at(index);
	sources.at(index) = nullptr;
	return that;
}

unsigned Manager::Update()
{
	unsigned count = 0;
	for (Source *it : sources)
	{
		if (not it or it->Update())
		{
			++count;
		}
	}
	return count;
}

unsigned Manager::Size()
{
	return to_unsigned(sources.size());
}
