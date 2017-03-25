#include "Lux.hpp"
#include "Lua.hpp"
#include "SDL.hpp"
#include "XML.hpp"
#include "Window.hpp"
#include "Strings.hpp"
#include "Document.hpp"
#include <lux/lux.hpp>
#include <exception>

#define tointeger lua_tointeger(state, value)
#define tostring  lua_tostring(state, value)
#define toboolean lua_toboolean(state, value)


static int setattributes(lua_State *state)
{
	int m = 0;
	lua_pushnil(state);
	constexpr int index = 1;
	while (lua_next(state, index))
	{
		constexpr int value = -1;
		const char *key = lua_tostring(state, -2);

		if (not SDL_strcasecmp(key, "RED"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_RED_SIZE, tointeger))
		 {
		  SDL_perror("SDL_GL_RED_SIZE");
		 }
		}
		else
		if (not SDL_strcasecmp(key, "GREEN"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, tointeger))
		 {
		  SDL_perror("SDL_GL_GREEN_SIZE");
		 }
		}
		else
		if (not SDL_strcasecmp(key, "BLUE"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, tointeger))
		 {
		  SDL_perror("SDL_GL_BLUE_SIZE");
		 }
		}
		else
		if (not SDL_strcasecmp(key, "ALPHA"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, tointeger))
		 {
		  SDL_perror("SDL_GL_ALPHA_SIZE");
		 }
		}
		else
		if (not SDL_strcasecmp(key, "BUFFER"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, tointeger))
		 {
		  SDL_perror("SDL_GL_BUFFER_SIZE");
		 }
		}
		else
		if (not SDL_strcasecmp(key, "DOUBLEBUFFER"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, toboolean))
		 {
		  SDL_perror("SDL_GL_DOUBLEBUFFER");
		 }
		}
		else
		if (not SDL_strcasecmp(key, "DEPTH"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, tointeger))
		 {
		  SDL_perror("SDL_GL_DEPTH_SIZE");
		 }
		}
		else
		if (not SDL_strcasecmp(key, "STENCIL"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, tointeger))
		 {
		  SDL_perror("SDL_GL_STENCIL_SIZE");
		 }
		}
		else
		if (not SDL_strcasecmp(key, "ACCUM_RED"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, tointeger))
		 {
		  SDL_perror("SDL_GL_ACCUM_RED_SIZE");
		 }
		}
		else
		if (not SDL_strcasecmp(key, "ACCUM_GREEN"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, tointeger))
		 {
		  SDL_perror("SDL_GL_ACCUM_GREEN_SIZE");
		 }
		}
		else
		if (not SDL_strcasecmp(key, "ACCUM_BLUE"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, tointeger))
		 {
		  SDL_perror("SDL_GL_ACCUM_RED_SIZE");
		 }
		}
		else
		if (not SDL_strcasecmp(key, "ACCUM_ALPHA"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, tointeger))
		 {
		  SDL_perror("SDL_GL_ACCUM_GREEN_SIZE");
		 }
		}
		else
		if (not SDL_strcasecmp(key, "STEREO"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_STEREO, toboolean))
		 {
		  SDL_perror("SDL_GL_STEREO");
		 }
		}
		else
		if (not SDL_strcasecmp(key, "MULTISAMPLEBUFFERS"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, tointeger))
		 {
		  SDL_perror("SDL_GL_MULTISAMPLEBUFFERS");
		 }
		}
		else
		if (not SDL_strcasecmp(key, "MULTISAMPLESAMPLES"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, tointeger))
		 {
		  SDL_perror("SDL_GL_MULTISAMPLESAMPLES");
		 }
		}
		else
		if (not SDL_strcasecmp(key, "ACCELERATED"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, toboolean))
		 {
		  SDL_perror("SDL_GL_ACCELERATED_VISUAL");
		 }
		}
		else
		if (not SDL_strcasecmp(key, "MAJOR"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, tointeger))
		 {
		  SDL_perror("SDL_GL_CONTEXT_MAJOR_VERSION");
		 }
		}
		else
		if (not SDL_strcasecmp(key, "MINOR"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, tointeger))
		 {
		  SDL_perror("SDL_GL_CONTEXT_MINOR_VERSION");
		 }
		}
		else
		if (not SDL_strcasecmp(key, "SHARE"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, toboolean))
		 {
		  SDL_perror("SDL_GL_SHARE_WITH_CURRENT_CONTEXT");
		 }
		}
		if (not SDL_strcasecmp(key, "SRGB"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, toboolean))
		 {
		  SDL_perror("SDL_GL_FRAMEBUFFER_SRGB_CAPABLE");
		 }
		}
		else
		if (not SDL_strcasecmp(key, "DEBUG"))
		{
		 if (toboolean) m |= SDL_GL_CONTEXT_DEBUG_FLAG;
		}
		else
		if (not SDL_strcasecmp(key, "FORWARD_COMPATIBLE"))
		{
		 if (toboolean) m |= SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
		}
		else
		if (not SDL_strcasecmp(key, "ROBUST_ACCESS"))
		{
		 if (toboolean) m |= SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG;
		}
		else
		if (not SDL_strcasecmp(key, "RESET_ISOLATION"))
		{
		 if (toboolean) m |= SDL_GL_CONTEXT_RESET_ISOLATION_FLAG;
		}
		else
		if (not SDL_strcasecmp(key, "PROFILE_CORE"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE))
		 {
		  SDL_perror("SDL_GL_CONTEXT_PROFILE_CORE");
		 }
		}
		else
		if (not SDL_strcasecmp(key, "PROFILE_COMPATIBILITY"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY))
		 {
		  SDL_perror("SDL_GL_CONTEXT_PROFILE_COMPATIBILITY");
		 }
		}
		else
		if (not SDL_strcasecmp(key, "PROFILE_ES"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES))
		 {
		  SDL_perror("SDL_GL_CONTEXT_PROFILE_ES");
		 }
		}
		else
		{
			return luaL_error(state, BadArgument, key);
		}
		lua_pop(state, 1);
	}
	lua_pop(state, 1);

	if (m and not SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, m))
	{
	 SDL_perror("SDL_GL_CONTEXT_FLAGS");
	}

	return 0;
}

static int renderer(lua_State *state)
{
	const int index = lua_gettop(state);
	luaL_checktype(state, index, LUA_TTABLE);

	SDL_Window *window = nullptr;
	unsigned flags = 0;
	int driver = -1;

	lua_pushnil(state);
	while (lua_next(state, index))
	{
		constexpr int value = -1;
		const char *key = lua_tostring(state, -2);

		if (not SDL_strcasecmp(key, "WINDOW"))
		{
			window = lux_to<SDL_Window*>(state, value);
		}
		else
		if (not SDL_strcasecmp(key, "INDEX"))
		{
			driver = tointeger;
		}
		else
		if (not SDL_strcasecmp(key, "SOFTWARE"))
		{
			if (toboolean) flags |= SDL_RENDERER_SOFTWARE;
		}
		else
		if (not SDL_strcasecmp(key, "ACCLERATED"))
		{
			if (toboolean) flags |= SDL_RENDERER_ACCELERATED;
		}
		else
		if (not SDL_strcasecmp(key, "VSYNC"))
		{
			if (toboolean) flags |= SDL_RENDERER_PRESENTVSYNC;
		}
		else
		if (not SDL_strcasecmp(key, "TARGETTEXTURE"))
		{
			if (toboolean) flags |= SDL_RENDERER_TARGETTEXTURE;
		}
		else
		{
			return luaL_error(state, BadArgument, key);
		}
		lua_pop(state, 1);
	}
	lua_pop(state, 1);

	auto renderer = SDL_CreateRenderer(window, driver, flags);
	if (not renderer)
	{
		return Lua_GetError(state);
	}

	lux_push(state, renderer);

	return 1;
}

static int window(lua_State *state)
{
	const int index = lua_gettop(state);
	luaL_checktype(state, index, LUA_TTABLE);

	const char *pos[] = { "center", "undefined", nullptr };

	const char *title = nullptr;
	int x = SDL_WINDOWPOS_UNDEFINED;
	int y = SDL_WINDOWPOS_UNDEFINED;
	int w = 0;
	int h = 0;
	unsigned m = 0;

	lua_pushnil(state);
	while (lua_next(state, index))
	{
		constexpr int value = -1;
		const char *key = lua_tostring(state, -2);

		if (not SDL_strcasecmp(key, "TITLE"))
		{
			title = tostring;
		}
		else
		if (not SDL_strcasecmp(key, "X"))
		{
			if (lua_isinteger(state, value))
			{
				x = tointeger;
			}
			else
			switch (luaL_checkoption(state, value, nullptr, pos))
			{
			case 0:	x = SDL_WINDOWPOS_CENTERED;
				break;
			case 1: x = SDL_WINDOWPOS_UNDEFINED;
				break;
			}
		}
		else
		if (not SDL_strcasecmp(key, "Y"))
		{
			if (lua_isinteger(state, value))
			{
				y = tointeger;
			}
			else
			switch (luaL_checkoption(state, value, nullptr, pos))
			{
			case 0:	y = SDL_WINDOWPOS_CENTERED;
				break;
			case 1: y = SDL_WINDOWPOS_UNDEFINED;
				break;
			}
		}
		else
		if (not SDL_strcasecmp(key, "WIDTH"))
		{
			w = tointeger;
		}
		else
		if (not SDL_strcasecmp(key, "HEIGHT"))
		{
			h = tointeger;
		}
		else
		if (not SDL_strcasecmp(key, "FULLSCREEN"))
		{
			if (toboolean) m |= SDL_WINDOW_FULLSCREEN;
		}
		else
		if (not SDL_strcasecmp(key, "DESKTOP"))
		{
			if (toboolean) m |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		else
		if (not SDL_strcasecmp(key, "SHOWN"))
		{
			if (toboolean) m |= SDL_WINDOW_SHOWN;
		}
		else
		if (not SDL_strcasecmp(key, "HIDDEN"))
		{
			if (toboolean) m |= SDL_WINDOW_HIDDEN;
		}
		else
		if (not SDL_strcasecmp(key, "BORDERLESS"))
		{
			if (toboolean) m |= SDL_WINDOW_BORDERLESS;
		}
		else
		if (not SDL_strcasecmp(key, "RESIZABLE"))
		{
			if (toboolean) m |= SDL_WINDOW_RESIZABLE;
		}
		else
		if (not SDL_strcasecmp(key, "HIGHDPI"))
		{
			if (toboolean) m |= SDL_WINDOW_ALLOW_HIGHDPI;
		}
		else
		if (not SDL_strcasecmp(key, "MINIMIZED"))
		{
			if (toboolean) m |= SDL_WINDOW_MINIMIZED;
		}
		else
		if (not SDL_strcasecmp(key, "MAXIMIZED"))
		{
			if (toboolean) m |= SDL_WINDOW_MAXIMIZED;
		}
		else
		if (not SDL_strcasecmp(key, "INPUT_GRABBED"))
		{
			if (toboolean) m |= SDL_WINDOW_INPUT_GRABBED;
		}
		else
		if (not SDL_strcasecmp(key, "INPUT_FOCUS"))
		{
			if (toboolean) m |= SDL_WINDOW_INPUT_FOCUS;
		}
		else
		if (not SDL_strcasecmp(key, "MOUSE_FOCUS"))
		{
			if (toboolean) m |= SDL_WINDOW_MOUSE_FOCUS;
		}
		else
		{
			return luaL_error(state, BadArgument, key);
		}
		lua_pop(state, 1);
	}
	lua_pop(state, 1);

	lux_push(state, CreateWindow(title, x, y, w, h, m));
	const int value = lua_gettop(state);
	if (luaL_newmetatable(state, "Window"))
	{
		// TODO: add meta methods
	}
	lua_setmetatable(state, value);
	return 1;
}


static int document(lua_State *state)
{
	auto path = luaL_checkstring(state, 1);
	constexpr int schema = 2;
	auto encoding = luaL_optstring(state, 3, nullptr);
	try
	{
		Document doc(encoding);
		if (not doc.Load(state, schema, path))
		{
			return luaL_error(state, CannotLoadDocument, path);
		}
	}
	catch (std::exception &exception)
	{
		const auto error = exception.what();
		return luaL_error(state, CaughtException, error);
	}
	return 1;
}


signed Lux_Init(lua_State *state)
{
	lua_pushcfunction(state, setattributes);
	lua_setglobal(state, "SetAttributes");
	lua_pushcfunction(state, document);
	lua_setglobal(state, "Document");
	lua_pushcfunction(state, renderer);
	lua_setglobal(state, "Renderer");
	lua_pushcfunction(state, window);
	lua_setglobal(state, "Window");
	return 0;
}

