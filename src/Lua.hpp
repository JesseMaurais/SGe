#ifndef Lua_hpp
#define Lua_hpp

extern "C"
{
#include <lua5.3/lua.h>
#include <lua5.3/lualib.h>
#include <lua5.3/lauxlib.h>
}

extern lua_State *CoState;

signed Lua_SetError(lua_State *state);
signed Lua_GetError(lua_State *state);
void Lua_perror(lua_State *state, const char *prefix);

signed Lua_Init(const char *path);
void Lua_Quit();

int Lua_Yield(lua_State *state);
int Lua_Resume(lua_State *state);

#endif // file
