#include "Lua.hpp"
#include "Lux.hpp"
#include "SDL.hpp"
#include "Error.hpp"
#include <cstdlib>
#include <cstdio>

lua_State *State = nullptr;
lua_State *CoState = nullptr;

bool Lua::SetError(lua_State *state)
{
	const auto string = lua_tostring(state, -1);
	return 0 < SDL_SetError("%s", string);
}

signed Lua::GetError(lua_State *state)
{
	const auto string = SDL_GetError();
	return luaL_error(state, "%s", string);
}

void Lua::perror(lua_State *state, const char *prefix)
{
	auto string = lua_tostring(state, -1);
	SDL_Log("%s: %s", prefix, string);
}

bool Lua::Init(const char *path)
{
	State = luaL_newstate();
	if (not State)
	{
		return SDL::SetError(OutOfMemory);
	}

	luaL_checkversion(State);
	luaL_openlibs(State);

	if (Lux::Init(State))
	{
		return false;
	}

	CoState = lua_newthread(State);
	if (not CoState)
	{
		SDL::SetError(OutOfMemory);
		return false;
	}

	if (luaL_dofile(CoState, path))
	{
		Lua::perror(CoState, "luaL_dofile");
		return false;
	}

	return true;
}

void Lua::Quit(void)
{
	if (State)
	{
		lua_close(State);
		CoState = nullptr;
		State = nullptr;
	}
}

int Lua::Yield(lua_State *state)
{
	return lua_yield(state, 0);
}

int Lua::Resume(lua_State *state)
{
	bool requested = SDL_QuitRequested();
	lua_pushboolean(state, !requested);
	return lua_resume(state, nullptr, 1);
}

