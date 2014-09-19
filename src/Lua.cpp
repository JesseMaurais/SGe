#include "Lua.hpp"
#include "Lux.hpp"
#include "SDL.hpp"
#include <cstdlib>
#include <cstdio>

lua_State *State;
lua_State *Coroutine;

void Lua_perror(lua_State *state, const char *string)
{
	SDL_Log("%s: %s", string, lua_tostring(state, -1));
	lua_pop(state, 1);
}

signed Lua_Init()
{
	State = luaL_newstate();
	if (!State)
	{
	 return SDL_SetError("out of memory");
	}

	luaL_openlibs(State);
	if (luaL_dofile(State, "Configs"))
	{
	 Lua_perror(State, "luaL_dofile");
	 return SDL_SetError("cannot load Configs");
	}

	Coroutine = lua_newthread(State);
	if (!Coroutine)
	{
	 return SDL_SetError("out of memory");
	}

	return Lux_Init();
}

void Lua_Quit(void)
{
	if (State)
	{
	 lua_close(State);
	 State = nullptr;
	}
}

