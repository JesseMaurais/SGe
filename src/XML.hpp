#ifndef XML_hpp
#define XML_hpp

#include <expat.h>

struct SDL_RWops;

class XML
{
public:

	virtual ~XML();
	XML(char const *encoding = nullptr);
	bool Parse(SDL_RWops *ops);

  protected:

	virtual void Start(char const *name, char const **attributes) = 0;
	virtual void End(char const *name) = 0;

	XML_Parser parser;
};

#endif // file
