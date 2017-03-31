#ifndef Collect_hpp
#define Collect_hpp

#include <set>
#include <cassert>

template <typename Type> class Collect final
{
private:

	std::set<Type*> set;
	Collect() = default;

public:

	~Collect()
	{
		assert(Empty());
	}

	static Collect &Instance()
	{
		static Collect singleton;
		return singleton;
	}

	bool Add(Type *object)
	{
		assert(object);
		return set.insert(object).second;
	}

	bool Remove(Type *object)
	{
		assert(object);
		return set.erase(object) == 1;
	}

	bool Has(Type *object)
	{
		assert(object);
		return set.find(object) != set.end();
	}

	bool Empty()
	{
		return set.empty();
	}

	template <typename ...Args>
	void Call(void(Type::*method)(Args...), Args... args)
	{
		for (auto it : set)
		{
			(it->method)(args...);
		}
	}
};

#endif // file
