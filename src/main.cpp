#include "Lua.hpp"
#include "SDL.hpp"
#include "Event.hpp"
#include "Error.hpp"
#include "Strings.hpp"
#include "Command.hpp"
#include <cstdlib>
#include <cstdio>

static bool Quit = false;
static bool OnQuit(const SDL_Event &event)
{
	Quit = true;
	return true;
}

int main(int argc, char **argv)
{
	const char *program = argv[0];
	const char *script = argv[1];

	if (not script)
	{
		script = ConfigScript;
	}

	if (SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_perror("SDL_Init");
		return EXIT_FAILURE;
	}
	else
	if (std::atexit(SDL_Quit))
	{
		SDL_Log(CannotMakeExit, "SDL_Quit");
		return EXIT_FAILURE;
	}

	SetErrorHandler(nullptr);
	if (std::atexit(ResetErrorHandler))
	{
		SDL_Log(CannotMakeExit, "ResetErrorHandler");
	}

	if (Lua_Init(script))
	{
		SDL_perror("Lua_Init");
		return EXIT_FAILURE;
	}
	else
	if (std::atexit(Lua_Quit))
	{
		SDL_Log(CannotMakeExit, "Lua_Quit");
		return EXIT_FAILURE;
	}

	if (CommandInit(CommandPrompt))
	{
		SDL_perror("CommandInit");
		return EXIT_FAILURE;
	}
	else
	if (std::atexit(CommandQuit))
	{
		SDL_Log(CannotMakeExit, "CommandQuit");
		return EXIT_FAILURE;
	}

	PushEventHandler(SDL_QUIT, OnQuit);

	SDL_Event event;
	while (not Quit)
	{
		if (SDL_WaitEvent(&event))
		{
			Dispatch(event);
		}
		else
		{
			SDL_perror("SDL_WaitEvent");
		}
	}
	return EXIT_SUCCESS;
}

