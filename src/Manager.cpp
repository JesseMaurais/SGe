#include "Manager.hpp"
#include "std.hpp"

unsigned ManagedResources::Add(Source *that)
{
	assert(that);
	unsigned id = Size();
	sources.push_back(that);
	return id;
}

Source *ManagedResources::Remove(unsigned id)
{
	assert(Has(id));
	Source *that = sources.at(id);
	sources.at(id) = sources.back();
	sources.at(id)->id = id;
	sources.pop_back();
	return that;
}


bool ManagedResources::Has(unsigned id)
{
	return id < sources.size() and sources.at(id);
}

unsigned ManagedResources::Size()
{
	return to_unsigned(sources.size());
}

unsigned ManagedResources::UpdateSources()
{
	return UpdateSources(sources);
}

unsigned ManagedResources::UpdateSources(std::vector<Source*> const &sources)
{
	unsigned count = 0;
	for (auto const that : sources)
	{
		if (not that or that->UpdateSource())
		{
			++count;
		}
	}
	return count;
}

unsigned ManagedResources::UpdateSources(std::vector<unsigned> const &ids)
{
	unsigned count = 0;
	for (unsigned const id : ids)
	{
		Source *that = sources.at(id);
		if (not that or that->UpdateSource())
		{
			++count;
		}
	}
	return count;
}

