#ifndef __lxclass__
#define __lxclass__

#include "lxalloc.hpp"
#include "lxstack.hpp"

template <class C> struct lux_Class
{
	typedef lux_Type<C*> Type;

	static int __new(lua_State  *vm)
	{
		new (vm) C; // default construct
		luaL_setmetatable(vm, Type::name);
		return 1;
	}

	static int __delete(lua_State *vm)
	{
		C *obj = lux_to<C*>(vm, 1);
		obj->~C(); // destructor
		return 0;
	}

	static int __tostring(lua_State *vm)
	{
		lua_pushstring(vm, Type::name);
		return 1;
	}

	static int open(lua_State *vm)
	{
		luaL_newmetatable(vm, Type::name);
		int meta = lua_gettop(vm);
		lua_newtable(vm);
		int tab = lua_gettop(vm);

		lua_pushliteral(vm, "__tostring");
		lua_pushcfunction(vm, __tostring);
		lua_settable(vm, meta);

		lua_pushliteral(vm, "__metatable");
		lua_pushvalue(vm, tab);
		lua_settable(vm, meta);

		lua_pushliteral(vm, "__index");
		lua_pushvalue(vm, tab);
		lua_settable(vm, meta);

		lua_pushliteral(vm, "__gc");
		lua_pushcfunction(vm, __delete);
		lua_settable(vm, meta);

		lua_register(vm, Type::name, __new);
		setfuncs(vm);

		lua_pop(vm, 2);
		return 0;
	}

	// This must be implemented per template instance

	static void setfuncs(lua_State *vm);
};


#endif // file

