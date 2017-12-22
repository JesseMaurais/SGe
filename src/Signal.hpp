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
	using Container = std::map<Slot, Observer>;
	using KeyValue = typename Container::value_type;

	bool Connect(Slot id, Observer observer)
	{
		auto pair = slots.insert_or_assign(id, observer);
		return pair.second; // true if not extant
	}

	bool Disconnect(Slot id)
	{
		return 1 == slots.erase(id);
	}

	void Disconnect()
	{
		slots.clear();
	}

	template <typename Filter>
	void Emit(Filter &&filter)
	{
		std::for_each(slots.begin(), slots.end(), filter);
	}

	bool Move(Slot from, Slot to)
	{
		auto const it = slots.find(from);
		bool const unique = Connect(to, it->second);
		slots.erase(it);
		return unique;
	}

	void Merge(Signal const & that)
	{
		slots.merge(that.slots);
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

	virtual ~Slot()
	{
		subject.Disconnect(this);
	}

	Slot(Subject &sub, Observer observer) : subject(sub)
	{
		subject.Connect(this, observer);
	}

private:

	Subject &subject;
};

namespace sys::sig
{
	using Slot = Slot<int>;
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
