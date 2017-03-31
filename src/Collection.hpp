#ifndef Collection_hpp
#define Collection_hpp

class CollectionInterface;

class Collected
{
private:
	
	CollectionInterface &collection;
	
public:
	
	Collected(CollectionInterface &collection);
	virtual ~Collected();
};

class CollectionInterface : Collected
{
public:
	CollectionInterface()
	~CollectionInterface()
	virtual bool Has(Collected *object) = 0;
};

#include <set>
#include <cassert>

template <typename Type> class Collection : CollectionInterface
{
private:

	std::set<Type*> set;
	Collection() = default;
	
	bool Has(Collected *object) override
	{
		auto upcast = dynamic_cast<Type*>(object);
		return upcast and Has(upcast);
	}

public:

	~Collection()
	{
		assert(Empty());
	}

	static Collection &Instance()
	{
		static Collection singleton;
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
	
	template <typename ...args>
	unsigned Count(bool(Type::*method)(Args...), Args... args)
	{
		unsigned count = 0;
		for (auto it : set)
		{
			if ((it->method)(args...))
			{
				++count;
			}
		}
		return count;
	}
};

using CollectionManager = Collection<CollectionInterface>;

#endif // file
