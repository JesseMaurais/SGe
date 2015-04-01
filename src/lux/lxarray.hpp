#ifndef __lxarray__
#define __lxarray__

#include "lxalloc.hpp"
#include "lxstack.hpp"

int luxopen_array(lua_State *vm);

template <class C> struct lux_Array
{
	typedef lux_Type<C*> Type;

	static C &value(lua_State *vm, int index, int offset)
	{
		C *buf = lux_to<C*>(vm, index);
		index = luaL_checkint(vm, offset);
		return buf[index];
	}

	static int __len(lua_State *vm)
	{
		lua_pushinteger(vm, lua_rawlen(vm, 1)/sizeof(C));
		return 1;
	}

	static int __new(lua_State *vm)
	{
		size_t sz;
		C *buf = NULL;
		const char *str = NULL;

		switch (lua_type(vm, 1))
		{
		case LUA_TNUMBER:
			sz = lua_tointeger(vm, 1);
			new (vm) C [sz];
			break;
		case LUA_TSTRING:
			// should make it utf8 aware
			str = lua_tolstring(vm, 1, &sz);
			buf = new (vm) C [sz];
			for (int i = 0; i <= sz; ++i)
			{
			 buf[i] = str[i];
			}
			break;
		case LUA_TTABLE:
			sz = luaL_len(vm, 1);
			buf = new (vm) C [sz];
			lua_pushnil(vm); // first key
			for (int i = 0; lua_next(vm, 1); ++i)
			{
			 buf[i] = lux_to<C>(vm, -1);
			 lua_pop(vm, 1);
			}
			break;
		default:
			return luaL_argerror(vm, 1, "number, string, table");
		};
		luaL_setmetatable(vm, Type::name);
		return 1;
	}

	static int __index(lua_State *vm)
	{
		lux_push<C>(vm, value(vm, 1, 2));
		return 1;
	}

	static int __newindex(lua_State *vm)
	{
		value(vm, 1, 2) = lux_to<C>(vm, 3);
		return 0;
	}

	static int __add(lua_State *vm)
	{
		lux_push<C*>(vm, &value(vm, 1, 2));
		return 1;
	}

	static int __concat(lua_State *vm)
	{
		lua_pushfstring
		(vm
		,"%s%s"
		,lua_tostring(vm, 1)
		,lux_to<char*>(vm, 2) // better be null terminated
		);
		return 1;
	}

	static int __tostring(lua_State *vm)
	{
		lua_pushstring(vm, Type::name);
		return 1;
	}

	static int open(lua_State *vm)
	{
		luaL_newmetatable(vm, Type::name);

		lua_pushliteral(vm, "__tostring");
		lua_pushcfunction(vm, __tostring);
		lua_settable(vm, -3);

		lua_pushliteral(vm, "__newindex");
		lua_pushcfunction(vm, __newindex);
		lua_settable(vm, -3);

		lua_pushliteral(vm, "__index");
		lua_pushcfunction(vm, __index);
		lua_settable(vm, -3);

		lua_pushliteral(vm, "__concat");
		lua_pushcfunction(vm, __concat);
		lua_settable(vm, -3);

		lua_pushliteral(vm, "__add");
		lua_pushcfunction(vm, __add);
		lua_settable(vm, -3);

		lua_pushliteral(vm, "__len");
		lua_pushcfunction(vm, __len);
		lua_settable(vm, -3);

		lua_register(vm, Type::name, __new);

		lua_pop(vm, 1);

		return 0;
	}
};

#endif // file

