#include "Options.hpp"
#include "Command.hpp"
#include "Strings.hpp"
#include "Event.hpp"
#include "Lua.hpp"
#include "SDL.hpp"
#include <cstdlib>

static bool Quit = false;
static bool OnQuit(const SDL_Event &event)
{
	Quit = true;
	return true;
}

int main(int argc, char **argv)
{
	const char *program = argv[0];
	const char *script = ConfigScript;
	unsigned media = 0;

	if (argc > 1)
	{
		CommandLine cmd;
		do {
			cmd = ParseCommandLine(argc, argv);
			switch (cmd.opt)
			{
			case Option::Configs:
				script = cmd.arg;
				break;
			case Option::Video:
				media |= SDL_INIT_VIDEO;
				break;
			case Option::Video:
				media |= SDL_INIT_AUDIO;
				break;
			case Option::Help:
				return EXIT_SUCCESS;
			}
		}
		while (cmd.opt);
	}

	if (not media)
	{
		media = SDL_INIT_EVERYTHING;
	}

	if (SDL_Init(media))
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

