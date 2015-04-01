#include "lux.hpp"

void *operator new (size_t size, lua_State *vm)
{
	return lua_newuserdata(vm, size);
}

void *operator new [] (size_t size, lua_State *vm)
{
	return lua_newuserdata(vm, size);
}

int lux_stackdump(lua_State *vm)
{
	int top = lua_gettop(vm);

	for (int i = 1; i <= top; ++i)
	{
		const char *str = luaL_typename(vm, i);

		switch (lua_type(vm, i))
		{
		case LUA_TNIL:
		case LUA_TNONE:
		case LUA_TFUNCTION:
			printf("[%d] %s\n", i, str);
			break;
		case LUA_TNUMBER:
			printf("[%d] %s: %f\n", i, str, lua_tonumber(vm, i));
			break;
		case LUA_TBOOLEAN:
			printf("[%d] %s: %d\n", i, str, lua_toboolean(vm, i));
			break;
		case LUA_TSTRING:
			printf("[%d] %s: %s\n", i, str, lua_tostring(vm, i));
			break;
		case LUA_TTHREAD:
			printf("[%d] %s: %p\n", i, str, lua_tothread(vm, i));
			break;
		case LUA_TLIGHTUSERDATA:
			printf("[%d] %s: %p\n", i, str, lua_touserdata(vm, i));
			break;
		case LUA_TTABLE:
		case LUA_TUSERDATA:
			if (luaL_callmeta(vm, i, "__tostring"))
			{
			 str = lua_tostring(vm, -1);
			 printf("[%d] %s: %p\n", i, str, lua_topointer(vm, i));
			 lua_pop(vm, 1);
			}
			else
			 printf("[%d] %s: %p\n", i, str, lua_topointer(vm, i));
			break;
		}
	}
	return 0;
}


template <> void lux_push<int>(lua_State *vm, int value)
{
	lua_pushinteger(vm, value);
}
template <> void lux_push<char>(lua_State *vm, char value)
{
	lua_pushinteger(vm, value);
}
template <> void lux_push<short>(lua_State *vm, short value)
{
	lua_pushinteger(vm, value);
}
template <> void lux_push<long>(lua_State *vm, long value)
{
	lua_pushinteger(vm, value);
}
template <> void lux_push<float>(lua_State *vm, float value)
{
	lua_pushnumber(vm, value);
}
template <> void lux_push<double>(lua_State *vm, double value)
{
	lua_pushnumber(vm, value);
}
template <> void lux_push<std::string>(lua_State *vm, std::string string)
{
	const char *cstring = string.c_str();
	if (cstring)
	 lua_pushstring(vm, cstring);
	else
	 lua_pushnil(vm);
}
template <> void lux_push<std::vector<std::string>>(lua_State *state, std::vector<std::string> list)
{
	lua_newtable(state);
	int table = lua_gettop(state);
	for (int item = 0; item < list.size(); ++item)
	{
	 lux_push(state, list[item]);
	 lua_rawseti(state, table, item);
	}
}

template <> void lux_push<unsigned int>(lua_State *vm, unsigned int value)
{
	lua_pushunsigned(vm, value);
}
template <> void lux_push<unsigned char>(lua_State *vm, unsigned char value)
{
	lua_pushunsigned(vm, value);
}
template <> void lux_push<unsigned short>(lua_State *vm, unsigned short value)
{
	lua_pushunsigned(vm, value);
}
template <> void lux_push<unsigned long>(lua_State *vm, unsigned long value)
{
	lua_pushunsigned(vm, value);
}

template <> int lux_to<int>(lua_State *vm, int index)
{
	return lua_tointeger(vm, index);
}
template <> char lux_to<char>(lua_State *vm, int index)
{
	return lua_tointeger(vm, index);
}
template <> short lux_to<short>(lua_State *vm, int index)
{
	return lua_tointeger(vm, index);
}
template <> long lux_to<long>(lua_State *vm, int index)
{
	return lua_tointeger(vm, index);
}
template <> float lux_to<float>(lua_State *vm, int index)
{
	return lua_tonumber(vm, index);
}
template <> double lux_to<double>(lua_State *vm, int index)
{
	return lua_tonumber(vm, index);
}
template <> std::string lux_to<std::string>(lua_State *vm, int index)
{
	return std::string(lua_tostring(vm, index));
}
template <> std::vector<std::string> lux_to<std::vector<std::string>>(lua_State *state, int index)
{
	std::vector<std::string> list;
	if (lua_istable(state, index))
	{
	 lua_pushnil(state);
	 while (lua_next(state, index))
	 {
	  list.push_back(lux_to<std::string>(state, -1));
	  lua_pop(state, 1);
	 }
	}
	else
	 luaL_argerror(state, index, "table expected");
	return list;
}

template <> unsigned int lux_to<unsigned int>(lua_State *vm, int index)
{
	return lua_tounsigned(vm, index);
}
template <> unsigned char lux_to<unsigned char>(lua_State *vm, int index)
{
	return lua_tounsigned(vm, index);
}
template <> unsigned short lux_to<unsigned short>(lua_State *vm, int index)
{
	return lua_tounsigned(vm, index);
}
template <> unsigned long lux_to<unsigned long>(lua_State *vm, int index)
{
	return lua_tounsigned(vm, index);
}


template <> const char *lux_Array<int>::Type::name = "int";
template <> const char *lux_Array<char>::Type::name = "char";
template <> const char *lux_Array<short>::Type::name = "short";
template <> const char *lux_Array<long>::Type::name = "long";
template <> const char *lux_Array<float>::Type::name = "float";
template <> const char *lux_Array<double>::Type::name = "double";

template <> const char *lux_Array<unsigned int>::Type::name = "uint";
template <> const char *lux_Array<unsigned char>::Type::name = "uchar";
template <> const char *lux_Array<unsigned short>::Type::name = "ushort";
template <> const char *lux_Array<unsigned long>::Type::name = "ulong";

int luxopen_array(lua_State *vm)
{
	lux_Array<int>::open(vm);
	lux_Array<char>::open(vm);
	lux_Array<short>::open(vm);
	lux_Array<long>::open(vm);
	lux_Array<float>::open(vm);
	lux_Array<double>::open(vm);

	lux_Array<unsigned int>::open(vm);
	lux_Array<unsigned char>::open(vm);
	lux_Array<unsigned short>::open(vm);
	lux_Array<unsigned long>::open(vm);

	return 0;
}

