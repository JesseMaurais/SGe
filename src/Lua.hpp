#ifndef __Lua__
#define __Lua__

#include <lua5.2/lua.hpp>

void Lua_perror(lua_State *state, const char *string);

extern lua_State *State; // Virtual Machine
extern lua_State *Coroutine; // Pseudo Thread

signed Lua_Init();
void Lua_Quit();

#endif
