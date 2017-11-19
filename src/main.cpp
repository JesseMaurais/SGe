#include "Desktop.hpp"
#include "JavaScript.hpp"
#include "Error.hpp"
#include "Event.hpp"
#include "SDL.hpp"
#include <cstdlib>
#include <getopt.h>
#include <iostream>

namespace
{
	enum Option
	{
		Unknown  = '?',
		Help     = 'h',
		Quit     = 'q',
		Configs  = 'f',
		Media    = 'm',
		End      = '\0'
	};

	struct CommandLineOption
	{
		int opt;
		char const *arg;
	};

	CommandLineOption ParseCommandLine(int argc, char **argv)
	{
		const struct option options[] =
		{
		{ "help"    , no_argument       , nullptr , Option::Help    },
		{ "configs" , required_argument , nullptr , Option::Configs },
		{ "media"   , optional_argument , nullptr , Option::Media   },
		{ "quit"    , no_argument       , nullptr , Option::Quit    },
		{ nullptr   , 0                 , nullptr , Option::End     }
		};

		CommandLineOption cmd;
		int index, opt = getopt_long_only(argc, argv, "", options, &index);
		switch (opt)
		{
		case -1:
			cmd.opt = Option::End;
			cmd.arg = nullptr;
			break;
		case '?':
			cmd.opt = Option::Unknown;
			cmd.arg = argv[index];
			break;
		default:
			cmd.opt = opt;
			cmd.arg = optarg;
		}
		return cmd;
	}

	void PrintCommandLineOptions()
	{
		std::cout << "I should get around to this" << std::endl;
	}
}

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
	js::Engine engine;
	SDL::ProcessEvents();
	return EXIT_SUCCESS;
}

