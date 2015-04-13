#include "Lua.hpp"
#include "Lux.hpp"
#include "SDL.hpp"
#include "XML.hpp"
#include <cassert>
#include "lux/lux.cpp"

// C++ XML class to Lua

static int loadxml(lua_State *state)
{
	const char *path = luaL_checkstring(state, -1);
	assert(path);
	XML obj;
	obj.Load(path);
	return 0;
}

// SDL2 event structure to Lua

template <> const char *lux_Index<SDL_Event>::Type::name = "Event";
static int next(lua_State *state)
{
	return lua_yield(state, 0);
}


signed Lux_Init()
{
	// Register C arrays

	luxopen_array(State);

	// Register XML class

	lua_register(State, "XML", loadxml);

	// Create global SDL event variable

	lux_Index<SDL_Event>::open(State);
	lux_Index<SDL_Event>::get["next"] = next;
	lux_getter(SDL_Event, type);
	#define param(name) lux_Index<SDL_Event>::pseudo<SDL_##name>(#name)
	param(QUIT);
	param(WINDOWEVENT);
	param(KEYDOWN);
	param(KEYUP);
	param(MOUSEMOTION);
	param(MOUSEBUTTONDOWN);
	param(MOUSEBUTTONUP);
	param(MOUSEWHEEL);
	param(DROPFILE);
	param(PRESSED);
	param(RELEASED);
	Event = new (State) SDL_Event;
	if (!Event)
	{
	 return SDL_SetError("out of memory");
	}
	luaL_setmetatable(State, lux_Index<SDL_Event>::Type::name);
	lua_setglobal(State, "event");

	// Done

	return 0;
}

