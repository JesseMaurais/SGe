#include "Desktop.hpp"
#include "Error.hpp"
#include "Event.hpp"
#include "Args.hpp"
#include "SDL.hpp"
#include <cstdlib>

int main(int argc, char **argv)
{
	SDL::ScopedAssertHandler handler;
	{
		if (argc > 1)
		{
			CommandLineOption cmd;
			do
			{
				cmd = ParseCommandLine(argc, argv);
				switch (cmd.opt)
				{
				case Option::Help:
					PrintCommandLineOptions();
					return EXIT_SUCCESS;
				}
			}
			while (Option::End != cmd.opt);
		}

		if (SDL_Init(SDL_INIT_EVERYTHING))
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
	SDL_assert(not "Message Box");
	SDL::ProcessEvents();
	return EXIT_SUCCESS;
}

