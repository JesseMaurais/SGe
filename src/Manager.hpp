#ifndef Manager_hpp
#define Manager_hpp

#include "Signal.hpp"
#include "std.hpp"

template <typename Type> class Manager : private Signal<unsigned, unsigned>
{
public:

	// Regenerate all from their sources
	void Initialize()
	{
		Generate(data); // include added
		added.clear(); // done adding
		// Sync with data
		UpdateSources();
	}

	// Destroy all resources
	void Release()
	{
		// Destroy any removed too
		stl::append(removed, data);
		Destroy(removed); // retain data
		removed.clear(); // done removing
	}

	// Update any added or removed sources
	void Update()
	{
		// Destroy any removed
		Destroy(removed);
		removed.clear();
		// Reserve space for added
		auto const size = added.size();
		std::vector<Type> newdata(size);
		// Generate new data for added
		Generate(newdata);
		Replace(newdata);
		// Update the added from their sources
		UpdateSources(added);
		added.clear();
	}

	// Capture the id and generate in queue later
	unsigned Add(Observer observer)
	{
		unsigned const index = to_unsigned(data.size() + added.size());
		Signal::Connect(index, observer);
		added.push_back(index);
		QueueUpdate();
		return index;
	}

	// Capture the id and destroy in queue later
	void Remove(unsigned index)
	{
		Signal::Disconnect(index);
		removed.push_back(data.at(index));
		QueueUpdate();
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
	
	void UpdateSources()
	{
		Signal::Emit([](auto const &pair)
		{
			pair.second(pair.first);
		});
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
