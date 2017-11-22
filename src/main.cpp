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
			Strict   = 's',
			Buffer   = 'z',
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
		{ "strict"   , no_argument       , nullptr , Option::Strict   },
		{ "buffer"   , required_argument , nullptr , Option::Buffer   },
		{ "bytecode" , no_argument       , nullptr , Option::ByteCode },
		{ "video"    , optional_argument , nullptr , Option::Video    },
		{ "help"     , no_argument       , nullptr , Option::Help     },
		{ "quit"     , no_argument       , nullptr , Option::Quit     },
		{ nullptr    , 0                 , nullptr , Option::End      }
		};

		Option arg;
		int index, opt = getopt_long(argc, argv, "", options, &index);
		switch (opt)
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
			arg.param = decltype(arg.param)(opt);
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
		jerry_init_flag_t jerry = JERRY_INIT_EMPTY;
		unsigned media = SDL_INIT_EVENTS;
		std::size_t buffer = BUFSIZ;
		char *video = nullptr;
		bool strict = false;

		bool done = argc > 1;
		while (not done)
		{
			auto arg = NextOption(argc, argv);
			switch (arg.param)
			{
			case Option::End:
				done = true;
				break;

			case Option::Strict:
				strict = true;
				break;

			case Option::Buffer:
				buffer = std::stoi(arg.value);
				break;

			case Option::ByteCode:
				jerry = JERRY_INIT_SHOW_OPCODES;
				break;

			case Option::Video:
				media |= SDL_INIT_VIDEO;
				video = arg.value;
				break;

			case Option::Quit:
				SDL::SendUserEvent(EscapeEvent);
				break;

			case Option::Unknown:
				SDL::Log(InvalidArgument, arg.value);
				// no break
	
			case Option::Help:
				PrintHelp();
				return EXIT_SUCCESS;
			}
		}

		if (not js::Init(jerry))
		{
			SDL::perror("js::Init");
			return EXIT_FAILURE;
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

		if (InitCommand(strict, buffer))
		{
			SDL::perror("InitCommand");
			return EXIT_FAILURE;
		}
	}
	SDL::ProcessEvents();
	return EXIT_SUCCESS;
}

