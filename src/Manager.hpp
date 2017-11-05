#ifndef Manager_hpp
#define Manager_hpp

#include "std.hpp"
#include "Source.hpp"
#include <vector>

class ManagerCommon : public Resources
{
public:

	unsigned Add(Source *that) override;
	Source *Remove(unsigned id) override;
	bool Has(unsigned id);
	unsigned UpdateSources();
	unsigned Size();

protected:

	unsigned UpdateSources(std::vector<Source*> const &sources);
	unsigned UpdateSources(std::vector<unsigned> const &ids);

private:

	std::vector<Source*> sources;
};

template <typename Type> class Manager : public ManagerCommon
{
public:

	// Regenerate all from their sources
	bool Initialize()
	{
		bool ok = true;
		// Current will match data sources
		auto const size = Size();
		ids.resize(size);
		if (size > 0)
		{
			Generate(ids);
			// Update from data sources
			ok = UpdateSources() == size;
		}
		return ok;
	}

	// Destroy all resources
	bool Release()
	{
		// Current should match data sources
		auto const size = Size();
		bool ok = ids.size() == size;
		// Destroy current and removed
		stl::append(ids, removed);
		if (not ids.empty())
		{
			Destroy(ids);
			ids.clear();
		}
		return ok;
	}

	// Update any added or removed sources
	bool Update()
	{
		// Delete any removed
		Destroy(removed);
		removed.clear();
		// Reserve space for added
		auto const size = added.size();
		std::vector<Type> newids(size);
		// Generate new for added
		Generate(newids);
		stl::append(ids, newids);
		// Update the new from their sources
		bool ok = UpdateSources(added) == size;
		added.clear();
		return ok;
	}

	// Get the internal resource data
	Type &Data(unsigned index)
	{
		return ids.at(index);
	}

protected:

	virtual void Generate(std::vector<Type> &ids) = 0;
	virtual void Destroy(std::vector<Type> const &ids) = 0;
	virtual bool SendUpdate() = 0;

private:

	std::vector<Type> ids;
	std::vector<unsigned> added;
	std::vector<Type> removed;

	bool NeedUpdate()
	{
		// Only need update if we have pending changes
		return not added.empty() or not removed.empty();
	}

	bool QueueUpdate()
	{
		// Only send the update message there is not one pending
		return not NeedUpdate() or SendUpdate();
	}

	// Capture the id and add in queue later
	unsigned Add(Source* that) override
	{
		QueueUpdate();
		unsigned index = Manager::Add(that);
		added.push_back(index);
		return index;
	}

	// Capture the id and remove in queue later
	Source* Remove(unsigned index) override
	{
		QueueUpdate();
		Source *that = Manager::Remove(index);
		removed.push_back(ids.at(index));
		// Replace back like Manager does
		ids.at(index) = ids.back();
		ids.pop_back();
		return that;
	}
};

#endif // file
