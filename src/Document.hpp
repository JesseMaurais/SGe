#ifndef Document_hpp
#define Document_hpp

#include "XML.hpp"
#include <stack>

struct lua_State;

struct Document : XML
{
	using XML::XML;

	bool Load(lua_State *state, int schema, SDL_RWops *ops);
	bool Load(lua_State *state, int schema, const char *path);

  protected:

	virtual void Start(const char *name, const char **attributes) override;
	virtual void End(const char *name) override;

  private:

	using Stack = std::stack<int>;

	lua_State *state;
	Stack index;
	int schema;
	int root;
};

#endif // file
