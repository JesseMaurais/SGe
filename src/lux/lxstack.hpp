#ifndef __lxstack__
#define __lxstack__

#include "lua.hpp"
#include <string>
#include <vector>

int lux_stackdump(lua_State *state);

template <class C> struct lux_Type
{
	static const char *name;
};

template <class T> struct lux_Strip
{
	typedef T Type;
};

template <class T> struct lux_Strip<const T*>
{
	typedef T *Type;
};

template <class C> void lux_push(lua_State *state, C value)
{
	union {
	 void *addr;
	 C obj;
	};
	obj = value;
	lua_pushlightuserdata(state, addr);
	luaL_setmetatable(state, lux_Type<C>::name);
}

template <class C> C lux_to(lua_State *state, int index)
{
	union {
	 void *addr;
	 C obj;
	};
	addr = lua_touserdata(state, index);
	return obj;
}

template <> void lux_push<int>(lua_State *state, int value);
template <> void lux_push<char>(lua_State *state, char value);
template <> void lux_push<short>(lua_State *state, short value);
template <> void lux_push<long>(lua_State *state, long value);
template <> void lux_push<float>(lua_State *state, float value);
template <> void lux_push<double>(lua_State *state, double value);
template <> void lux_push<std::string>(lua_State *state, std::string value);
template <> void lux_push<std::vector<std::string>>(lua_State *state, std::vector<std::string> value);

template <> void lux_push<unsigned int>(lua_State *state, unsigned int value);
template <> void lux_push<unsigned char>(lua_State *state, unsigned char value);
template <> void lux_push<unsigned short>(lua_State *state, unsigned short value);
template <> void lux_push<unsigned long>(lua_State *state, unsigned long value);

template <> int lux_to<int>(lua_State *state, int index);
template <> char lux_to<char>(lua_State *state, int index);
template <> short lux_to<short>(lua_State *state, int index);
template <> long lux_to<long>(lua_State *state, int index);
template <> float lux_to<float>(lua_State *state, int index);
template <> double lux_to<double>(lua_State *state, int index);
template <> std::string lux_to<std::string>(lua_State *state, int index);
template <> std::vector<std::string> lux_to<std::vector<std::string>>(lua_State *state, int index);

template <> unsigned int lux_to<unsigned int>(lua_State *state, int index);
template <> unsigned char lux_to<unsigned char>(lua_State *state, int index);
template <> unsigned short lux_to<unsigned short>(lua_State *state, int index);
template <> unsigned long lux_to<unsigned long>(lua_State *state, int index);



#endif // file

