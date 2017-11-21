#include "JavaScript.hpp"
#include "Command.hpp"
#include "Error.hpp"
#include "Event.hpp"
#include "SDL.hpp"

#include <iostream>
#include <getopt.h>

namespace
{
	struct Option
	{
		enum
		{
			Console  = 'c',
			ByteCode = 'b',
			Video    = 'v',
			Help     = 'h',
			Quit     = 'q',
			Unknown  = '?',
			End      = '\0'

		} param = Unknown;
		char *value = nullptr;
	};

	Option NextOption(int argc, char **argv)
	{
		const struct option options[] =
		{
		{ "console"  , optional_argument , nullptr , Option::Console  },
		{ "bytecode" , no_argument       , nullptr , Option::ByteCode },
		{ "video"    , optional_argument , nullptr , Option::Video    },
		{ "help"     , no_argument       , nullptr , Option::Help     },
		{ "quit"     , no_argument       , nullptr , Option::Quit     },
		{ nullptr    , 0                 , nullptr , Option::End      }
		};

		Option arg;
		int index;
		switch (getopt_long(argc, argv, "", options, &index))
		{
		case -1:
			arg.param = Option::End;
			arg.value = nullptr;
			break;
		case '?':
			arg.param = Option::Unknown;
			arg.value = argv[index];
			break;
		default:
			arg.param = opt;
			arg.value = optarg;
		}
		return arg;
	}

	void PrintHelp()
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
			jerry_init_flags_t jerry = JERRY_INIT_EMPTY;
			Uint32 media = SDL_INIT_EVENTS;
			char const *video = nullptr;
			char const *prompt = nullptr;

			do
			{
				auto arg = NextOption(argc, argv);
				switch (arg.param)
				{
				case Option::Console:
					prompt = arg.value ? arg.value : String(CommandPrompt);
					break;

				case Option::ByteCode:
					jerry |= JERRY_INIT_SHOW_OPCODES;
					break;

				case Option::Video:
					media |= SDL_INIT_VIDEO;
					video = arg.value;
					break;

				case Option::Quit:
					SDL::SendUserEvent(EscapeEvent);
					break;

				case Option::Unkown:
					SDL::Log(InvalidArgument, arg.value);
					// no break
	
				case Option::Help:
					PrintHelpt();
					return EXIT_SUCCESS;
				}
			}
			while (Option::End != arg.param);
		}

		if (not js::Init(jerry))
		{
			SDL::perror("js::Init");
			reeturn EXIT_FAILURE;
		}

		if (not SDL::Init(media))
		{
			SDL::perror("SDL::Init");
			return EXIT_FAILURE;
		}

		if (video and SDL_VideoInit(video))
		{
			SDL::perror(video);
			return EXIT_FAILURE;
		}

		if (InitCommand(prompt))
		{
			SDL::perror("InitCommand");
			return EXIT_FAILURE;
		}
	}
	SDL::ProcessEvents();
	return EXIT_SUCCESS;
}

