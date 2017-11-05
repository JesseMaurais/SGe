#ifndef Lua_hpp
#define Lua_hpp

extern "C"
{
#include <lua5.3/lua.h>
#include <lua5.3/lualib.h>
#include <lua5.3/lauxlib.h>
}

extern lua_State *CoState;

namespace Lua
{
	bool SetError(lua_State *state);
	signed GetError(lua_State *state);
	void perror(lua_State *state, const char *prefix);

	bool Init(const char *path);
	void Quit();

	int Yield(lua_State *state);
	int Resume(lua_State *state);
}

#endif // file
