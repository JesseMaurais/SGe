#ifndef Manager_hpp
#define Manager_hpp

#include "Source.hpp"
#include "std.hpp"

class ManagedResources : public Resources
{
public:

	virtual ~ManagedResources() = default;
	unsigned Add(Source *that) override;
	Source *Remove(unsigned id) override;
	Source *At(unsigned id) const;
	bool Has(unsigned id) const;
	unsigned Size() const;

protected:

	unsigned UpdateSources() const;
	unsigned UpdateSources(std::vector<Source*> const &sources) const;
	unsigned UpdateSources(std::vector<unsigned> const &ids) const;

private:

	std::vector<Source*> sources;
};


template <typename Type> class Manager : public ManagedResources
{
public:

	// Regenerate all from their sources
	bool Initialize()
	{
		Generate(data); // include added
		added.clear(); // done adding
		// Verify that sources sync with data
		return UpdateSources() == data.size();
	}

	// Destroy all resources
	bool Release()
	{
		// Destroy any removed too
		stl::append(removed, data);
		Destroy(removed); // retain data
		removed.clear(); // done removing
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
		// Generate new data for added
		Generate(newdata);
		Replace(newdata);
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

	// Locate corresponding source of data
	Source *Find(Type const &value) const
	{
		auto const it = stl::find(data, value);
		if (data.end() != it)
		{
			auto const index = std::distance(data.begin(), it);
			return ManagedResources::At(index);
		}
		return nullptr;
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
		unsigned index = ManagedResources::Add(that);
		added.push_back(index);
		QueueUpdate();
		return index;
	}

	// Capture the id and destroy in queue later
	Source* Remove(unsigned index) override
	{
		Source *that = ManagedResources::Remove(index);
		removed.push_back(data.at(index));
		stl::replace(added, Size(), index);
		data.at(index) = data.back();
		data.pop_back();
		QueueUpdate();
		return that;
	}

	// Replace data with new data at added indices
	void Replace(std::vector<Type> const &newdata)
	{
		assert(newdata.size() == added.size());
		for (std::size_t it = 0; it < added.size(); ++it)
		{
			unsigned const id = added.at(it);
			data.at(id) = newdata.at(it);
		}
	}
};

#endif // file
