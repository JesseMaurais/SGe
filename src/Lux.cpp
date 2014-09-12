#include "lux/lux.cpp"
#include "Lua.hpp"
#include "Lux.hpp"
#include "SDL.hpp"
#include "Geom.hpp"
#include "Shader.hpp"

template <> const char *lux_Class<Box>::Type::name = "Box";
template <> void lux_Class<Box>::setfuncs(lua_State *state)
{
	luaL_Reg regs[] =
	{
	{"Position", lux_wrap(Box::Position)},
	{"Rotation", lux_wrap(Box::Rotation)},
	{"LinearVel", lux_wrap(Box::LinearVel)},
	{"AngularVel", lux_wrap(Box::AngularVel)},
	{NULL, NULL}
	};
	luaL_setfuncs(state, regs, 0);
}
template <> const char *lux_Class<Shader>::Type::name = "Shader";
template <> void lux_Class<Shader>::setfuncs(lua_State *state)
{
	luaL_Reg regs[] =
	{
	{"LoadFragmentSource", lux_wrap(Shader::LoadFragmentSource)},
	{"LoadVertexSource", lux_wrap(Shader::LoadVertexSource)},
	{"LinkProgram", lux_wrap(Shader::LinkProgram)},
	{"UseProgram", lux_wrap(Shader::UseProgram)},
	{NULL, NULL}
	};
	luaL_setfuncs(state, regs, 0);
}

template <> const char *lux_Index<SDL_Event>::Type::name = "Event";

static int next(lua_State *vm)
{
	return lua_yield(vm, 0);
}

signed Lux_Init()
{
	luxopen_array(State);

	lux_Class<Shader>::open(State);
	lux_Class<Box>::open(State);

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

