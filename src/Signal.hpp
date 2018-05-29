#ifndef Signal_hpp
#define Signal_hpp

#include <algorithm>
#include <functional>
#include <map>

template <typename Slot, typename... Args> class Signal
{
public:

	using Signature = void(Args...);
	using Observer = std::function<Signature>;
	using Container = std::multimap<Slot, Observer>;

	virtual void Connect(Slot id, Observer observer)
	{
		slots.emplace(id, observer);
	}

	virtual void Disconnect(Slot id)
	{
		slots.erase(id);
	}

	void Disconnect()
	{
		slots.clear();
	}

	bool Has(Slot id)
	{
		return slots.find(id) != slots.end();
	}

	template <typename Filter> void Emit(Filter &&filter)
	{
		std::for_each(slots.begin(), slots.end(), filter);
	}

	template <typename Filter> void Emit(Slot id, Filter &&filter)
	{
		const auto pair = slots.equal_range(id);
		std::for_each(pair.first, pair.second, filter);
	}

private:

	Container slots;
};


template <typename... Args> class Slot
{
public:

	using Subject = Signal<Slot*, Args...>;
	using Signature = typename Subject::Signature;
	using Observer = typename Subject::Observer;

	Slot(Subject *sub, Observer observer) : subject(sub)
	{
		subject->Connect(this, observer);
	}

	~Slot()
	{
		subject->Disconnect(this);
	}

	operator bool() const
	{
		return nullptr != subject;
	}

private:

	Subject *subject;
};

namespace sys::sig
{
	using Slot = Slot<int>;
	using Subject = Slot::Subject;
	using Signature = Slot::Signature;
	using Observer = Slot::Observer;

	struct ScopedHandler : Slot
	{
		ScopedHandler(int signo, Observer handler);
		~ScopedHandler();
		operator bool();

	private:

		Signature *handler;
		int signo;
	};
}

#endif // file