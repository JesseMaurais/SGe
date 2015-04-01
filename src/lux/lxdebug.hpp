#ifndef __lxdebug__
#define __lxdebug__

#include "lua.hpp"

struct lux_Debug : lua_Debug
{
	lux_Debug(lua_State *vm, unsigned level=0)
	{
		while (lua_getstack(vm, level, this)) --level;
	}

	bool info(lua_State *vm, const char *what="n")
	{
		return lua_getinfo(vm, what, this);
	}
};

#endif // file

