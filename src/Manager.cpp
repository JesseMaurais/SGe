#include "Manager.hpp"
#include "std.hpp"

unsigned ResourcesCommon::Add(Source *that)
{
	assert(that);
	unsigned id = Size();
	sources.push_back(that);
	return id;
}

Source *ResourcesCommon::Remove(unsigned id)
{
	assert(Has(id));
	auto const that = sources.at(id);
	sources.at(id) = sources.back();
	sources.at(id)->id = id;
	sources.pop_back();
	return that;
}

unsigned ResourcesCommon::UpdateSources(std::vector<Source*> const &sources)
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

unsigned ResourcesCommon::UpdateSources(std::vector<unsigned> const &ids)
{
	unsigned count = 0;
	for (auto const id : ids)
	{
		auto const that = sources.at(id);
		if (not that or that->UpdateSource())
		{
			++count;
		}
	}
	return count;
}

unsigned ResourcesCommon::UpdateSources()
{
	return UpdateSources(sources);
}

bool ResourcesCommon::Has(unsigned index)
{
	return index < sources.size() and sources.at(index);
}

unsigned ResourcesCommon::Size()
{
	return to_unsigned(sources.size());
}
