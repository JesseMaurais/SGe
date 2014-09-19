#include "lux/lux.cpp"
#include "Lua.hpp"
#include "Lux.hpp"
#include "SDL.hpp"
#include "Geom.hpp"

template <> const char *lux_Class<Geometry>::Type::name = "Geometry";
template <> void lux_Class<Geometry>::setfuncs(lua_State *state)
{
	luaL_Reg regs[] =
	{
	{"Position", lux_wrap(Geometry::Position)},
	{"Rotation", lux_wrap(Geometry::Rotation)},
	{"LinearVel", lux_wrap(Geometry::LinearVel)},
	{"AngularVel", lux_wrap(Geometry::AngularVel)},
	{"Box", lux_wrap(Geometry::Box)},
	{NULL, NULL}
	};
	luaL_setfuncs(state, regs, 0);
}

template <> const char *lux_Index<SDL_Event>::Type::name = "Event";

static int next(lua_State *state)
{
	return lua_yield(state, 0);
}

signed Lux_Init()
{
	luxopen_array(State);

	lux_Class<Geometry>::open(State);
	lux_Class<Shader>::open(State);

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
	return 0;
}

