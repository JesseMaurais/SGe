#include "Manager.hpp"
#include "std.hpp"
#include "SDL.hpp"

unsigned Manager::Update()
{
	unsigned count = 0;
	for (auto const it : sources)
	{
		if (not it or it->Update())
		{
			++count;
		}
	}
	return count;
}

unsigned Manager::Add(Source *that)
{
	unsigned index;
	auto const size = sources.size();
	for (index = 0; index < size; ++index)
	{
		if (not sources[index])
		{
			sources[index] = that;
			return index;
		}
	}
	SDL_assert(size == index);
	sources.push_back(that);
	return index;
}

Source *Manager::Remove(unsigned index)
{
	SDL_assert(Has(index));
	auto const that = sources.at(index);
	sources.at(index) = nullptr;
	return that;
}

bool Manager::Has(unsigned index)
{
	return index < sources.size() and sources[index];
}

unsigned Manager::Size()
{
	return to_unsigned(sources.size());
}
