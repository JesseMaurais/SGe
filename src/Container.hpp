#ifndef Container_hpp
#define Container_hpp

#include <set>
#include <utility>
#include "SDL.hpp"

template <typename Type> class Container
{
private:

	std::set<Type*> set;
	Container() = default;

public:

	static Container &Instance()
	{
		static Container singleton;
		return singleton;
	}

	bool Add(Type *object)
	{
		SDL_assert(object);
		return set.insert(object).second;
	}

	bool Remove(Type *object)
	{
		SDL_assert(object);
		return set.erase(object) == 1;
	}

	bool Contains(Type *object)
	{
		SDL_assert(object);
		return set.find(object) != set.end();
	}

	template <typename ...Args>
	void ForwardEach(void(Type::*method)(Args...), Args... args)
	{
		for (auto it : set)
		{
			(it->*method)(std::forward<Args>(args)...);
		}
	}
};

template <typename Type> class Contained
{
private:

	using ContainerType = Container<Type>;

public:

	Contained()
	{
		SDL_verify(ContainerType::Instance().Add(this));
	}

	virtual ~Contained()
	{
		SDL_verify(ContainerType::Instance().Remove(this));
	}

	virtual bool Update() = 0;
};

#endif // file
