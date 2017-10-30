#include "Manager.hpp"
#include "std.hpp"

unsigned Manager::Add(Source *that)
{
	assert(that);
	unsigned id = Size();
	sources.push_back(that);
	return id;
}

Source *Manager::Remove(unsigned id)
{
	assert(Has(id));
	auto const that = sources.at(id);
	sources.at(id) = sources.back();
	sources.at(id)->id = id;
	sources.pop_back();
	return that;
}

unsigned Manager::Update()
{
	return Update(sources);
}

unsigned Manager::Update(std::vector<Source*> &sources)
{
	unsigned count = 0;
	for (auto const that : sources)
	{
		if (not that or that->Update())
		{
			++count;
		}
	}
	return count;
}

unsigned Manager::Update(std::vector<unsigned> &ids)
{
	unsigned count = 0;
	for (auto const id : ids)
	{
		auto const that = sources.at(id);
		if (not that or that->Update())
		{
			++count;
		}
	}
	return count;
}

bool Manager::Has(unsigned index)
{
	return index < sources.size() and sources.at(index);
}

unsigned Manager::Size()
{
	return to_unsigned(sources.size());
}
