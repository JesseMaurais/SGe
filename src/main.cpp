#include "Lua.hpp"
#include "SDL.hpp"
#include "ODE.hpp"
#include "OpenGL.hpp"
#include "OpenAL.hpp"
#include <cstdlib>
#include <cstdio>

extern void FindFaces();

int main(int argc, char **argv)
{
	if (argc < 2)
	{
	 puts("usage: SGe <lua-script>");
	 return EXIT_SUCCESS;
	}

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER))
	{
	 SDL_perror("SDL_Init");
	 return EXIT_SUCCESS;
	}
	else
	 atexit(SDL_Quit);

	if (Lua_Init())
	{
	 SDL_perror("Lua_Init");
	 return EXIT_SUCCESS;
	}
	else
	 atexit(Lua_Quit);

	if (OpenGL_Init())
	{
	 SDL_perror("OpenGL_Init");
	 return EXIT_SUCCESS;
	}
	else
	 atexit(OpenGL_Quit);

	if (OpenAL_Init())
	{
	 SDL_perror("OpenAL_Init");
	 return EXIT_SUCCESS;
	}
	else
	 atexit(OpenAL_Quit);

	if (ODE_Init())
	{
	 SDL_perror("ODE_Init");
	 return EXIT_SUCCESS;
	}
	else
	 atexit(ODE_Quit);

	if (luaL_loadfile(Coroutine, argv[1]))
	{
	 Lua_perror(Coroutine, "luaL_loadfile");
	 return EXIT_SUCCESS;
	}
	else
	 atexit(Lua_Quit);

	gluLookAt(0, 5, 2,  0, 0, 0,  0, 1, 0);
	glClearColor(1.0, 1.0, 1.0, 0.0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDisable(GL_CULL_FACE);

	SDL_bool quit = SDL_FALSE;
	do
	{
		if (!SDL_WaitEvent(Event))
		{
		 SDL_perror("SDL_WaitEvent");
		 continue;
		}

		if (HaveEvent(UPDATE))
		{
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

			if (ODE_Lock())
			{
			 SDL_Log("cannot lock");
			 continue;
			}

			Update(Space);

			if (ODE_Unlock())
			{
			 SDL_Log("cannot unlock");
			}

			SDL_GL_SwapWindow(Window);
		}
		else
		{
			bool requested = SDL_QuitRequested();
			lua_pushboolean(Coroutine, !requested);

			if (ODE_Lock())
			{
			 SDL_Log("cannot lock");
			 continue;
			}

			int error = lua_resume(Coroutine, NULL, 1);

			if (ODE_Unlock())
			{
			 SDL_Log("cannot unlock");
			}

			switch (error)
			{
			default:
				Lua_perror(Coroutine, argv[1]);
			case LUA_OK:
				quit = SDL_TRUE;
				break;
			case LUA_YIELD:
				continue;
			}
		}
	}
	while (!quit);
}

