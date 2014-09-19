#include "SDL.hpp"
#include "Lua.hpp"
#include "OpenGL.hpp"
#include <cstdlib>

static SDL_GLContext Context;

signed OpenGL_Init()
{
	const char *title = nullptr;
	int x = SDL_WINDOWPOS_UNDEFINED;
	int y = SDL_WINDOWPOS_UNDEFINED;
	int w = 800;
	int h = 600;
	int m = SDL_WINDOW_OPENGL;

	lua_getglobal(State, "Video");
	int table = lua_gettop(State);

	if (!lua_isnil(State, table))
	{
	 lua_pushnil(State);
	 while (lua_next(State, table))
	 {
		const char *key = lua_tostring(State, -2);
		#define tointeger lua_tointeger(State, -1)
		#define toboolean lua_toboolean(State, -1)
		#define tostring  lua_tostring(State, -1)

		if (!SDL_strcasecmp(key, "DEVICE"))
		{
		 if (SDL_VideoInit(tostring))
		 {
		  SDL_perror("SDL_VideoInit");
		 }
		 else
		  atexit(SDL_VideoQuit);
		}
		else
		if (!SDL_strcasecmp(key, "ALPHA"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, tointeger))
		 {
		  SDL_perror("SDL_GL_SetAttribute");
		 }
		}
		else
		if (!SDL_strcasecmp(key, "RED"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_RED_SIZE, tointeger))
		 {
		  SDL_perror("SDL_GL_SetAttribute");
		 }
		}
		else
		if (!SDL_strcasecmp(key, "GREEN"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, tointeger))
		 {
		  SDL_perror("SDL_GL_SetAttribute");
		 }
		}
		else
		if (!SDL_strcasecmp(key, "BLUE"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, tointeger))
		 {
		  SDL_perror("SDL_GL_SetAttribute");
		 }
		}
		else
		if (!SDL_strcasecmp(key, "DEPTH"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, tointeger))
		 {
		  SDL_perror("SDL_GL_SetAttribute");
		 }
		}
		else
		if (!SDL_strcasecmp(key, "STENCIL"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, tointeger))
		 {
		  SDL_perror("SDL_GL_SetAttribute");
		 }
		}
		else
		if (!SDL_strcasecmp(key, "BUFFER"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, tointeger))
		 {
		  SDL_perror("SDL_GL_SetAttribute");
		 }
		}
		else
		if (!SDL_strcasecmp(key, "DOUBLEBUFFER"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, toboolean))
		 {
		  SDL_perror("SDL_GL_SetAttribute");
		 }
		}
		else
		if (!SDL_strcasecmp(key, "ACCELERATED"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, toboolean))
		 {
		  SDL_perror("SDL_GL_SetAttribute");
		 }
		}
		else
		if (!SDL_strcasecmp(key, "MAJOR"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, tointeger))
		 {
		  SDL_perror("SDL_GL_SetAttribute");
		 }
		}
		else
		if (!SDL_strcasecmp(key, "MINOR"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, tointeger))
		 {
		  SDL_perror("SDL_GL_SetAttribute");
		 }
		}
		else
		if (!SDL_strcasecmp(key, "PROFILE_CORE"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE))
		 {
		  SDL_perror("SDL_GL_SetAttribute");
		 }
		}
		else
		if (!SDL_strcasecmp(key, "PROFILE_COMPATIBILITY"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY))
		 {
		  SDL_perror("SDL_GL_SetAttribute");
		 }
		}
		else
		if (!SDL_strcasecmp(key, "PROFILE_ES"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES))
		 {
		  SDL_perror("SDL_GL_SetAttribute");
		 }
		}
		else
		if (!SDL_strcasecmp(key, "TITLE"))
		{
		 title = tostring;
		}
		else
		if (!SDL_strcasecmp(key, "COLUMN"))
		{
		 x = tointeger;
		}
		else
		if (!SDL_strcasecmp(key, "ROW"))
		{
		 y = tointeger;
		}
		else
		if (!SDL_strcasecmp(key, "WIDTH"))
		{
		 w = tointeger;
		}
		else
		if (!SDL_strcasecmp(key, "HEIGHT"))
		{
		 h = tointeger;
		}
		else
		if (!SDL_strcasecmp(key, "FULLSCREEN"))
		{
		 if (toboolean) m |= SDL_WINDOW_FULLSCREEN;
		}
		else
		if (!SDL_strcasecmp(key, "DESKTOP"))
		{
		 if (toboolean) m |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		else
		if (!SDL_strcasecmp(key, "SHOWN"))
		{
		 if (toboolean) m |= SDL_WINDOW_SHOWN;
		}
		else
		if (!SDL_strcasecmp(key, "HIDDEN"))
		{
		 if (toboolean) m |= SDL_WINDOW_HIDDEN;
		}
		else
		if (!SDL_strcasecmp(key, "BORDERLESS"))
		{
		 if (toboolean) m |= SDL_WINDOW_BORDERLESS;
		}
		else
		if (!SDL_strcasecmp(key, "RESIZABLE"))
		{
		 if (toboolean) m |= SDL_WINDOW_RESIZABLE;
		}
		else
		if (!SDL_strcasecmp(key, "ALLOW_HIGHDPI"))
		{
		 if (toboolean) m |= SDL_WINDOW_ALLOW_HIGHDPI;
		}
		else
		if (!SDL_strcasecmp(key, "MINIMIZED"))
		{
		 if (toboolean) m |= SDL_WINDOW_MINIMIZED;
		}
		else
		if (!SDL_strcasecmp(key, "MAXIMIZED"))
		{
		 if (toboolean) m |= SDL_WINDOW_MAXIMIZED;
		}
		else
		if (!SDL_strcasecmp(key, "INPUT_GRABBED"))
		{
		 if (toboolean) m |= SDL_WINDOW_INPUT_GRABBED;
		}
		else
		if (!SDL_strcasecmp(key, "INPUT_FOCUS"))
		{
		 if (toboolean) m |= SDL_WINDOW_INPUT_FOCUS;
		}
		else
		if (!SDL_strcasecmp(key, "MOUSE_FOCUS"))
		{
		 if (toboolean) m |= SDL_WINDOW_MOUSE_FOCUS;
		}
		/*
		else
		if (!SDL_strcasecmp(key, "MOUSE_CAPTURE"))
		{
		 if (toboolean) m |= SDL_WINDOW_MOUSE_CAPTURE;
		}
		*/
		else
		{
		 SDL_Log("Video: %s does not match", key);
		}

		lua_pop(State, 1);
	 }
	}
	lua_pop(State, 1);

	Window = SDL_CreateWindow(title, x, y, w, h, m);
	if (!Window)
	{
	 SDL_perror("SDL_CreateWindow");
	 return SDL_SetError("cannot create rendering window");
	}

	Context = SDL_GL_CreateContext(Window);
	if (!Context)
	{
	 SDL_DestroyWindow(Window);
	 SDL_perror("SDL_GL_CreateContext");
	 return SDL_SetError("cannot create rendering context");
	}
	
	if (SDL_GL_MakeCurrent(Window, Context))
	{
	 SDL_DestroyWindow(Window);
	 SDL_GL_DeleteContext(Context);
	 SDL_perror("SDL_GL_MakeCurrent");
	 return SDL_SetError("cannot bind context to window");
	}
	
	GLenum error = glewInit();
	if (error)
	{
	 SDL_DestroyWindow(Window);
	 SDL_GL_DeleteContext(Context);
	 return SDL_SetError("glewInit: %s", glewGetErrorString(error));
	}

	double aspect = double(w) / (h ? h : 1);

	glMatrixMode(GL_PROJECTION);
	gluPerspective(60.0, aspect, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	
	return 0;
}

void OpenGL_Quit()
{
	SDL_GL_DeleteContext(Context);
	SDL_DestroyWindow(Window);
}

