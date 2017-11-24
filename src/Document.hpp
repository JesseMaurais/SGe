#ifndef Document_hpp
#define Document_hpp

#include <jerryscript.h>

class Document
{
public:

	Document();
	~Document();
	bool Load(SDL_RWops *ops);
	bool Load(char const *path);

private:

	jerry_value_t const self;
};

#endif // file
