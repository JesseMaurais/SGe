#ifndef Controller_hpp
#define Controller_hpp

#include <map>
#include <vector>
#include "SDL.hpp"

template <typename Type> class Controller
{
private:

	std::vector<Type> vector;
	std::map<unsigned, unsigned> map;

public:

	bool Has(unsigned id)
	{
		return map.find(id) != map.end();
	}

	Type &Find(unsigned id)
	{
		SDL_assert(Has(id));
	}


};

#endif // file
