#include "Lua.hpp"
#include "Lux.hpp"
#include "SDL.hpp"
#include "Strings.hpp"
#include <cstdlib>
#include <cstdio>

lua_State *State = nullptr;
lua_State *CoState = nullptr;

signed Lua_SetError(lua_State *state)
{
	const auto string = lua_tostring(state, -1);
	return SDL_SetError("%s", string);
}

signed Lua_GetError(lua_State *state)
{
	const auto string = SDL_GetError();
	return luaL_error(state, "%s", string);
}

void Lua_perror(lua_State *state, const char *prefix)
{
	auto string = lua_tostring(state, -1);
	SDL_Log("%s: %s", prefix, string);
}

signed Lua_Init(const char *path)
{
	State = luaL_newstate();
	if (not State)
	{
		return SDL_SetError(String(OutOfMemory));
	}

	luaL_checkversion(State);
	luaL_openlibs(State);

	if (Lux_Init(State))
	{
		return SDL_SetError("Lux_Init");
	}

	CoState = lua_newthread(State);
	if (not CoState)
	{
		return SDL_SetError(String(OutOfMemory));
	}

	if (luaL_dofile(CoState, path))
	{
		if (SDL_strcasecmp(path, String(Config)))
		{
			Lua_perror(CoState, "luaL_dofile");
			return SDL_SetError(String(CannotLoadScript), path);
		}
	}

	return 0;
}

void Lua_Quit(void)
{
	if (State)
	{
		lua_close(State);
		CoState = nullptr;
		State = nullptr;
	}
}

int Lua_Yield(lua_State *state)
{
	return lua_yield(state, 0);
}

int Lua_Resume(lua_State *state)
{
	bool requested = SDL_QuitRequested();
	lua_pushboolean(state, !requested);
	return lua_resume(state, nullptr, 1);
}

