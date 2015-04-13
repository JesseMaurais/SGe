#ifndef __lxbufsz__
#define __lxbufsz__

#include "lua.hpp"

struct lux_Buffer : luaL_Buffer
{
	lux_Buffer(lua_State *vm) : str(NULL)
	{
		luaL_buffinit(vm, this);
	}

	lux_Buffer(lua_State *vm, size_t sz)
	{
		luaL_buffinitsize(vm, this, sz);
	}

	void add(size_t sz)
	{
		luaL_addsize(this, sz);
	}

	void add(const char *str)
	{
		luaL_addstring(this, str);
	}

	void add(const char *str, size_t sz)
	{
		luaL_addlstring(this, str, sz);
	}

	void addchar(char byte)
	{
		luaL_addchar(this, byte);
	}

	void addvalue(int index)
	{
		lua_pushvalue(L, index);
		add();
	}

	void addvalue(void)
	{
		luaL_addvalue(this);
	}

	char *prep(void)
	{
		return luaL_prepbuffer(this);
	}

	char *prep(size_t sz)
	{
		return luaL_prepbuffsize(this, sz);
	}

	void push(void)
	{
		luaL_pushresult(this);
	}

	void push(size_t sz)
	{
		luaL_pushresultsize(this, sz);
	}
};

#endif // file

