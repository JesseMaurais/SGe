#ifndef Manager_hpp
#define Manager_hpp

#include "std.hpp"
#include "Source.hpp"
#include <vector>

class ResourceManager : public Resources
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

template <typename Type> class Manager : public ResourceManager
{
public:

	// Regenerate all from their sources
	bool Initialize()
	{
		Generate(data); // includes added
		added.clear(); // done adding now
		// Verify that sources sync with data
		return UpdateSources() == data.size();
	}

	// Destroy all resources
	bool Release()
	{
		// Destroy any removed too
		stl::append(removed, data);
		Destroy(removed); // retain data memory
		removed.clear(); // done removing now
		// Verify sources sync with data
		return Size() == data.size();
	}

	// Update any added or removed sources
	bool Update()
	{
		// Destroy any removed
		Destroy(removed);
		removed.clear();
		// Reserve space for added
		unsigned const size = added.size();
		std::vector<Type> newdata(size);
		// Generate new for added
		Generate(newdata);
		stl::append(data, newdata);
		// Update the added from their sources
		bool const ok = UpdateSources(added) == size;
		added.clear();
		return ok;
	}

	// Get the internal resource data
	Type &Data(unsigned index)
	{
		return data.at(index);
	}

protected:

	virtual void Generate(std::vector<Type> &data) = 0;
	virtual void Destroy(std::vector<Type> const &data) = 0;
	virtual bool SendUpdate() = 0;

private:

	std::vector<Type> data;
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

	// Capture the id and generate in queue later
	unsigned Add(Source* that) override
	{
		unsigned index = Manager::Add(that);
		added.push_back(index);
		QueueUpdate();
		return index;
	}

	// Capture the id and destroy in queue later
	Source* Remove(unsigned index) override
	{
		Source *that = Manager::Remove(index);
		removed.push_back(data.at(index));
		data.at(index) = data.back();
		data.pop_back();
		QueueUpdate();
		return that;
	}
};

#endif // file
