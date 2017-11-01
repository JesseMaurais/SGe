#include "Command.hpp"
#include "Strings.hpp"
#include "Event.hpp"
#include "Args.hpp"
#include "Lua.hpp"
#include "SDL.hpp"
#include <cstdlib>

static bool Quit = false;
static bool OnQuit(const SDL_Event &event)
{
	SDL_assert(SDL_QUIT == event.type);
	Quit = true;
	return true;
}

int main(int argc, char **argv)
{
	PushEventHandler(SDL_QUIT, OnQuit);
	SDL::SetAssertHandler(nullptr);
	{
		unsigned media = SDL_INIT_EVENTS;
		char const *script = nullptr;

		if (argc > 1)
		{
			CommandLineOption cmd;
			do {
				cmd = ParseCommandLine(argc, argv);
				switch (cmd.opt)
				{
				case Option::Quit:
					Quit = true;
					break;
				case Option::Media:
					media |= SDL_INIT_EVERYTHING;
					break;
				case Option::Configs:
					script = cmd.arg;
					break;
				case Option::Help:
					return EXIT_SUCCESS;
				}
			}
			while (Option::End != cmd.opt);
		}

		if (SDL_Init(media))
		{
			SDL::perror("SDL_Init");
			return EXIT_FAILURE;
		}
		else
		if (std::atexit(SDL_Quit))
		{
			SDL_Log(String(CannotMakeExit), "SDL_Quit");
			return EXIT_FAILURE;
		}

		if (Lua_Init(script))
		{
			SDL::perror("Lua_Init");
			return EXIT_FAILURE;
		}
		else
		if (std::atexit(Lua_Quit))
		{
			SDL_Log(String(CannotMakeExit), "Lua_Quit");
			return EXIT_FAILURE;
		}

		if (CommandInit(String(CommandPrompt)))
		{
			SDL::perror("CommandInit");
			return EXIT_FAILURE;
		}
		else
		if (std::atexit(CommandQuit))
		{
			SDL_Log(String(CannotMakeExit), "CommandQuit");
			return EXIT_FAILURE;
		}
	}

	SDL_Event event;
	while (not Quit)
	{
		if (SDL_WaitEvent(&event))
		{
			Dispatch(event);
		}
		else
		{
			SDL::perror("SDL_WaitEvent");
		}
	}
	return EXIT_SUCCESS;
}

