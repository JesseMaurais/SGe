#ifndef sig_hpp
#define sig_hpp

#include <algorithm>
#include <functional>
#include <map>

namespace sig
{
	template <typename Slot, typename... Args> class event
	{
	public:

		using signature = void(Args...);
		using observer = std::function<signature>;
		using container = std::map<Slot, observer>;
		using count = typename container::size_type;

		void connect(Slot id, observer fn)
		{
			slots.emplace(id, fn);
		}

		count disconnect(Slot id)
		{
			return slots.erase(id);
		}

		void disconnect()
		{
			slots.clear();
		}

		bool find(Slot id) const
		{
			return slots.find(id) != slots.end();
		}

		void send(Args... args) const
		{
			send([&](auto pair){ pair.second(args...); });
		}

		template <typename Filter>
		void send(Filter &&filter) const
		{
			std::for_each(begin(slots), end(slots), filter);
		}

		template <typename Filter>
		void send(Slot id, Filter &&filter) const
		{
			const auto pair = slots.equal_range(id);
			std::for_each(pair.first, pair.second, filter);
		}

	private:

		container slots;
	};

	template <typename... Args> class slot
	{
	public:

		using subject = event<slot*, Args...>;
		using signature = typename subject::signature;
		using observer = typename subject::observer;

		slot(subject *ev, observer fn) : that(ev)
		{
			that->connect(this, fn);
		}

		~slot()
		{
			that->disconnect(this);
		}

	private:

		subject *that;
	};
};

namespace sys::sig
{
	struct handle : ::sig::slot<int>
	{
		operator bool();
		handle(int no, observer fn);
		~handle();

	private:

		signature *old;
		int signo;
	};

	using subject = handle::subject;
	using signature = handle::signature;
	using observer = handle::observer;
}

#endif // file
