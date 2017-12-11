#ifndef Signal_hpp
#define Signal_hpp

#include <functional>
#include <map>

template <typename Slot, typename... Args> class Signal
{
public:

	using Signature = void(Args...);
	using Observer = std::function<Signature>;
	using Container = std::map<Slot, Observer>;

	bool Connect(Slot &&id, Signature &&observer)
	{
		auto pair = slots.insert_or_assign(id, observer);
		return pair.second; // true if not extant
	}

	bool Disconnect(Slot &&id)
	{
		return 1 == slots.erase(id);
	}

	void Disconnect()
	{
		slots.clear();
	}

	void Emit(Args... args)
	{
		for (auto it : slots)
		{
			it.second(args...);
		}
	}

private:

	Container slots;
};


template <typename... Args> class Slot
{
public:

	using Subject = Signal<Slot, Args...>;
	using Observer = typename Subject::Observer;

	virtual ~Slot()
	{
		verify(signal.Disconnect(this));
	}

	Slot(Subject &sub, Observer &&callback) : signal(sub)
	{
		verify(signal.Connect(this, callback));
	}

	template <class Object>
	static Observer Bind(Object *that, void(Object::*method)(Args...))
	{
		return [that](Args... args) { that->method(args...); };
	}

private:

	Subject &signal;
};


#endif // file
