/** \file
 * Template for managing the addition and removal of resources in queue and
 * on reset. Uses signals & slots to notify observers when the reset occurs.
 */

#ifndef Manager_hpp
#define Manager_hpp

#include "Signal.hpp"
#include <cassert>
#include <vector>
#include <map>
#include <set>

/// Defers a slot's acquisition of managed resources to a moment in queue
template <typename Slot, typename Type> class Manager : public Signal<Slot, Type>
{
	using Signal = typename ::Signal<Slot, Type>;
	using Observer = typename Signal::Observer;

public:

	/// Regenerate all data, notifying observers
	void Initialize()
	{
		Add();
	}

	/// Destroy data but retain observer slots
	void Release()
	{
		Remove();
		Reset();
	}

	/// Update any added or removed slots
	void Update()
	{
		Remove();
		Add();
	}

	/// Capture the id and generate in queue later
	bool Connect(Slot id, Observer observer) override
	{
		// Belay if removal pending
		auto const it = removed.find(id);
		if (removed.end() != it)
		{
			removed.erase(it);
			return true;
		}
		// Otherwise connect and queue for update
		bool ok = Signal::Connect(id, observer) and QueueUpdate();
		added.insert(id);
		return ok;
	}

	/// Capture the id and destroy in queue later
	bool Disconnect(Slot id) override
	{
		// Belay if addition pending
		auto const it = added.find(id);
		if (added.end() != it)
		{
			added.erase(it);
			return true;
		}
		// Otherwise disconnect and queue for update
		bool ok = Signal::Disconnect(id) and QueueUpdate();
		removed.insert(id);
		return ok;
	}

protected:

	virtual void Generate(std::vector<Type> &data) const = 0;
	virtual void Destroy(std::vector<Type> const &data) const = 0;
	virtual bool SendUpdate() const = 0;

	/// Find the resource of a slot
	Type const &Find(Slot id) const
	{
		return map.at(id);
	}

private:

	std::set<Slot> added;
	std::set<Slot> removed;
	std::map<Slot, Type> map;

	// Generate and merge added into map
	void Add()
	{
		auto const size = added.size();
		std::vector<Type> data(size);
		Generate(data);
		for (auto const &key : added)
		{
			Type const &value = data.back();
			auto const pair = map.insert_or_assign(key, value);
			assert(pair.second);
			// Notify observer of its new resource value
			Signal::Emit(key, [value](auto const &pair)
			{
				pair.second(value);
			});
			data.pop_back();
		}
		added.clear();
	}

	// Destroy any removed from map
	void Remove()
	{
		std::vector<Type> data;
		for (Slot const &key : removed)
		{
			auto const it = map.find(key);
			if (map.end() != it)
			{
				data.push_back(it->second);
				map.erase(it);
			}
		}
		Destroy(data);
		removed.clear();
	}

	// Retain slots by moving into added
	void Reset()
	{
		std::vector<Type> data;
		for (auto const &pair : map)
		{
			added.insert(pair.first);
			data.push_back(pair.second);
		}
		Destroy(data);
		map.clear();
	}

	// Only need update if we no changes pending
	bool NeedUpdate() const
	{
		return added.empty() and removed.empty();
	}

	// Only send the update if none pending
	bool QueueUpdate() const
	{
		return not NeedUpdate() or SendUpdate();
	}
};

#endif // file
