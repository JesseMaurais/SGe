#include "SDL.hpp"
#include "Lua.hpp"
#include "OpenGL.hpp"

static SDL_GLContext Context;

signed OpenGL_Init()
{
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
		#define integer lua_tointeger(State, -1)
		#define boolean lua_toboolean(State, -1)

		if (!SDL_strcasecmp(key, "ALPHA"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, integer))
		 {
		  SDL_perror("SDL_GL_SetAttribute");
		 }
		}
		else
		if (!SDL_strcasecmp(key, "RED"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_RED_SIZE, integer))
		 {
		  SDL_perror("SDL_GL_SetAttribute");
		 }
		}
		else
		if (!SDL_strcasecmp(key, "GREEN"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, integer))
		 {
		  SDL_perror("SDL_GL_SetAttribute");
		 }
		}
		else
		if (!SDL_strcasecmp(key, "BLUE"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, integer))
		 {
		  SDL_perror("SDL_GL_SetAttribute");
		 }
		}
		else
		if (!SDL_strcasecmp(key, "DEPTH"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, integer))
		 {
		  SDL_perror("SDL_GL_SetAttribute");
		 }
		}
		else
		if (!SDL_strcasecmp(key, "STENCIL"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, integer))
		 {
		  SDL_perror("SDL_GL_SetAttribute");
		 }
		}
		else
		if (!SDL_strcasecmp(key, "BUFFER"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, integer))
		 {
		  SDL_perror("SDL_GL_SetAttribute");
		 }
		}
		else
		if (!SDL_strcasecmp(key, "DOUBLEBUFFER"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, boolean))
		 {
		  SDL_perror("SDL_GL_SetAttribute");
		 }
		}
		else
		if (!SDL_strcasecmp(key, "ACCELERATED"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, boolean))
		 {
		  SDL_perror("SDL_GL_SetAttribute");
		 }
		}
		else
		if (!SDL_strcasecmp(key, "MAJOR"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, integer))
		 {
		  SDL_perror("SDL_GL_SetAttribute");
		 }
		}
		else
		if (!SDL_strcasecmp(key, "MINOR"))
		{
		 if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, integer))
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
		if (!SDL_strcasecmp(key, "X"))
		{
		 x = integer;
		}
		else
		if (!SDL_strcasecmp(key, "Y"))
		{
		 y = integer;
		}
		else
		if (!SDL_strcasecmp(key, "WIDTH"))
		{
		 w = integer;
		}
		else
		if (!SDL_strcasecmp(key, "HEIGHT"))
		{
		 h = integer;
		}
		else
		if (!SDL_strcasecmp(key, "FULLSCREEN"))
		{
		 if (boolean) m |= SDL_WINDOW_FULLSCREEN;
		}
		else
		if (!SDL_strcasecmp(key, "DESKTOP"))
		{
		 if (boolean) m |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		else
		if (!SDL_strcasecmp(key, "SHOWN"))
		{
		 if (boolean) m |= SDL_WINDOW_SHOWN;
		}
		else
		if (!SDL_strcasecmp(key, "HIDDEN"))
		{
		 if (boolean) m |= SDL_WINDOW_HIDDEN;
		}
		else
		if (!SDL_strcasecmp(key, "BORDERLESS"))
		{
		 if (boolean) m |= SDL_WINDOW_BORDERLESS;
		}
		else
		if (!SDL_strcasecmp(key, "RESIZABLE"))
		{
		 if (boolean) m |= SDL_WINDOW_RESIZABLE;
		}
		else
		if (!SDL_strcasecmp(key, "ALLOW_HIGHDPI"))
		{
		 if (boolean) m |= SDL_WINDOW_ALLOW_HIGHDPI;
		}
		else
		if (!SDL_strcasecmp(key, "MINIMIZED"))
		{
		 if (boolean) m |= SDL_WINDOW_MINIMIZED;
		}
		else
		if (!SDL_strcasecmp(key, "MAXIMIZED"))
		{
		 if (boolean) m |= SDL_WINDOW_MAXIMIZED;
		}
		else
		if (!SDL_strcasecmp(key, "INPUT_GRABBED"))
		{
		 if (boolean) m |= SDL_WINDOW_INPUT_GRABBED;
		}
		else
		if (!SDL_strcasecmp(key, "INPUT_FOCUS"))
		{
		 if (boolean) m |= SDL_WINDOW_INPUT_FOCUS;
		}
		else
		if (!SDL_strcasecmp(key, "MOUSE_FOCUS"))
		{
		 if (boolean) m |= SDL_WINDOW_MOUSE_FOCUS;
		}
		/*
		else
		if (!SDL_strcasecmp(key, "MOUSE_CAPTURE"))
		{
		 if (boolean) m |= SDL_WINDOW_MOUSE_CAPTURE;
		}
		*/
		else
		{
		 SDL_Log("%s does not match", key);
		}

		lua_pop(State, 1);
	 }
	}
	lua_pop(State, 1);

	Window = SDL_CreateWindow("SGe", x, y, w, h, m);
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
	
	return 0;
}

void OpenGL_Quit()
{
	SDL_GL_DeleteContext(Context);
	SDL_DestroyWindow(Window);
}

