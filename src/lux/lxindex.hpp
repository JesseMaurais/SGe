#ifndef __lxindex__
#define __lxindex__

#include "lxalloc.hpp"
#include "lxstack.hpp"

#include <stdexcept>
#include <string>
#include <map>


typedef std::map<std::string, lua_CFunction> lux_Map;

template <class C> struct lux_Index
{
	typedef lux_Type<C*> Type;

	static lux_Map set, get;

	static int __len(lua_State *vm)
	{
		lua_pushinteger(vm, lua_rawlen(vm, 1)/sizeof(C));
		return 1;
	}

	static int __new(lua_State *vm)
	{
		void *addr;
		size_t sz;
		switch (lua_type(vm, 1))
		{
		case LUA_TNONE:
			new (vm) C;
			break;
		case LUA_TNUMBER:
			sz = lua_tointeger(vm, 1);
			new (vm) C [sz];
			break;
		case LUA_TTABLE:
			new (vm) C;
			luaL_setmetatable(vm, Type::name);
			lua_pushnil(vm); // first key
			while (lua_next(vm, 1))
			{
			 __newindex(vm);
			 lua_pop(vm, 1);
			}
			return 1;
		default:
			return luaL_argerror(vm, 1, "number, table, none");
		};
		luaL_setmetatable(vm, Type::name);
		return 1;
	}

	static int __index(lua_State *vm)
	{
		C *obj;
		int index;
		const char *key;
		switch (lua_type(vm, -1))
		{
		case LUA_TSTRING:
			key = lua_tostring(vm, -1);
			try {
			#if NDEBUG
			return get[key](vm);
			#else
			return get.at(key)(vm);
			#endif
			} catch (std::out_of_range error) {
			lua_pushnil(vm);
			return 1;
			}
		case LUA_TNUMBER:
			obj = lux_to<C*>(vm, -2);
			index = lua_tonumber(vm, -1);
			lux_push<C*>(vm, obj+index);
			return 1;
		default:
			return luaL_argerror(vm, 2, "string, number");
		}
	}

	static int __newindex(lua_State *vm)
	{
		const char *key = luaL_checkstring(vm, -2);
		try {
		#if NDEBUG
		return set[key](vm);
		#else
		return set.at(key)(vm);
		#endif
		} catch (std::out_of_range error) {
		return 1;
		}
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

		lua_pushliteral(vm, "__len");
		lua_pushcfunction(vm, __len);
		lua_settable(vm, -3);

		lua_register(vm, Type::name, __new);

		lua_pop(vm, 1);

		return 0;
	}

	/// Refer to a region of memory within userdata as a given type
	template <class X, int offset> static X &value(lua_State *vm, int index)
	{
		union
		{
		 char *addr;
		 C *obj;
		 X *ptr;
		};
		obj = lux_to<C*>(vm, index);
		addr += offset;
		return *ptr;
	}

	/// Change a region of memory within userdata as a given type
	template <class X, int offset> static int setter(lua_State *vm)
	{
		value<X, offset>(vm, -3) = lux_to<X>(vm, -1);
		return 0;
	}

	/// Create an index for a function to change a region of memory
	template <class X, int offset> static void setter(const char *name)
	{
		set[name] = setter<X, offset>;
	}	

	/// Query a region of memory within userdata as a given type
	template <class X, int offset> static int getter(lua_State *vm)
	{
		lux_push<X>(vm, value<X, offset>(vm, -2));
		return 1;
	}
	
	/// Create an index for a function to query a region of memory
	template <class X, int offset> static void getter(const char *name)
	{
		get[name] = getter<X, offset>;
	}

	/// Pseudo-index is a constant term
	template <int Num> static int pseudo(lua_State *vm)
	{
		lua_pushinteger(vm, Num);
		return 1;
	}
	/// Create pseudo-index
	template <int Num> static void pseudo(const char *name)
	{
		get[name] = pseudo<Num>;
	}

	/// Push the userdata as light with a new metatable
	template <class X> static int cast(lua_State *vm)
	{
		union
		{
		 C *obj;
		 X *ptr;
		};
		obj = lux_to<C*>(vm, -2);
		lux_push<X*>(vm, ptr);
		return 1;
	}

	/// Create an index to cast this userdata to another type
	template <class X> static void cast(const char *name)
	{
		get[name] = cast<X>;
	}

};

#define lux_cast(C,X) lux_Index<C>::cast<decltype(C::X)>(#X)
#define lux_pseudo(C) lux_Index<C>::pseudo<C>(#C)
#define lux_setter(C,X) lux_Index<C>::setter<decltype(C::X),offsetof(C,X)>(#X)
#define lux_getter(C,X) lux_Index<C>::getter<decltype(C::X),offsetof(C,X)>(#X)

template <class C> lux_Map lux_Index<C>::set;
template <class C> lux_Map lux_Index<C>::get;

#endif // file

