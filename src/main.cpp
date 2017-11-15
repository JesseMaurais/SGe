#include "Desktop.hpp"
#include "Command.hpp"
#include "Strings.hpp"
#include "Error.hpp"
#include "Event.hpp"
#include "Args.hpp"
#include "Lua.hpp"
#include "SDL.hpp"
#include "std.hpp"
#include <cstdlib>

int main(int argc, char **argv)
{
	SDL::ScopedAssertHandler handler;
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
			SDL::Log(CannotMakeExit, "SDL_Quit");
			return EXIT_FAILURE;
		}
	}
	SDL::ProcessEvents();
	return EXIT_SUCCESS;
}

