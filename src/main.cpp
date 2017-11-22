#include "JavaScript.hpp"
#include "Command.hpp"
#include "Error.hpp"
#include "Event.hpp"
#include "SDL.hpp"
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <getopt.h>

namespace
{
	struct Option
	{
		enum
		{
			InputFile = 'f',
			Buffer    = 'z',
			Strict    = 's',
			ByteCode  = 'b',
			Video     = 'v',
			Help      = 'h',
			Quit      = 'q',
			Unknown   = '?',
			Begin     = '*',
			End       = '\0'

		} param = Unknown;
		char *value = nullptr;
	};

	struct option const options[] =
	{
		{ "file"     , required_argument , nullptr , Option::InputFile },
		{ "buffer"   , required_argument , nullptr , Option::Buffer    },
		{ "strict"   , no_argument       , nullptr , Option::Strict    },
		{ "bytecode" , no_argument       , nullptr , Option::ByteCode  },
		{ "video"    , optional_argument , nullptr , Option::Video     },
		{ "help"     , no_argument       , nullptr , Option::Help      },
		{ "quit"     , no_argument       , nullptr , Option::Quit      },
		{ nullptr    , 0                 , nullptr , Option::End       }
	};

	Option NextOption(int argc, char **argv)
	{
		Option arg;
		opterr = false;
		int opt = getopt_long_only(argc, argv, "+", options, nullptr);
		switch (opt)
		{
		case -1:
			arg.param = Option::End;
			arg.value = nullptr;
			break;
		case '?':
			arg.param = optind ? Option::Begin : Option::Unknown;
			arg.value = argv[optind];
			break;
		default:
			arg.param = decltype(arg.param)(opt);
			arg.value = optarg;
		}
		return arg;
	}

	void PrintCommandLineOptions()
	{
		auto const argument = "ARG";

		for (struct option const &option : options)
		{
			if (not option.name)
			{
				return; // list is null terminated
			}

			// Give option name and indicate argument style

			std::string style;
			switch (option.has_arg)
			{
			case no_argument:
				stl::sprintf(style, "-%1 or --%2", (char) option.val, option.name);
				break;

			case optional_argument:
				stl::sprintf(style, "-%1 [%3] or --%2[=%3]", (char) option.val, option.name, argument);
				break;

			case required_argument:
				stl::sprintf(style, "-%1 %3 or --%2=%3", (char) option.val, option.name, argument);
				break;
			}

			// Give explanation of option

			std::string line;
			switch (option.val)
			{
			case Option::InputFile:
				line = "Read stream from file instead of standard input";
				break;

			case Option::Buffer:
				line = "Size of buffer used to read from input stream";
				break;

			case Option::Strict:
				line = "Be strict when parsing script";
				break;

			case Option::Video:
				line = "Name the video driver back-end to use";
				break;

			case Option::Help:
				line = "Show this help message";
				break;

			case Option::Quit:
				line = "Send the quit message immediately";
				break;

			default: // option not documented
				continue;
			}

			// Print formatted line to stdout

			stl::printf("%1\n\t%2\n", style, line);
		}
	}
}

int main(int argc, char **argv)
{
	SDL::ScopedAssertHandler handler;
	{
		// Option defaults without command line specification

		jerry_init_flag_t jerry = JERRY_INIT_EMPTY;
		unsigned media = SDL_INIT_EVENTS;
		std::size_t buffer = BUFSIZ;
		std::string input;
		std::string video;
		bool strict = false;

		// Parse command line options

		bool done = argc < 2;
		while (not done)
		{
			auto arg = NextOption(argc, argv);
			switch (arg.param)
			{
			case Option::Begin:
				// program name
				continue;

			case Option::End:
				done = true;
				continue;

			case Option::InputFile:
				input = arg.value;
				continue;

			case Option::Buffer:
				buffer = std::stoi(arg.value);
				continue;

			case Option::Strict:
				strict = true;
				continue;

			case Option::ByteCode:
				jerry = JERRY_INIT_SHOW_OPCODES;
				continue;

			case Option::Video:
				media |= SDL_INIT_VIDEO;
				video = arg.value;
				continue;

			case Option::Quit:
				SDL::SendUserEvent(EscapeEvent);
				continue;

			case Option::Unknown:
				SDL::Log(InvalidArgument, arg.value);
				// no break

			case Option::Help:
				PrintCommandLineOptions();
				return EXIT_SUCCESS;
			}
		}

		// Bind our interfaces to the script engine

		if (not js::Init(jerry))
		{
			SDL::perror("js::Init");
			return EXIT_FAILURE;
		}

		// Startup the minimum specified media back-ends

		if (not SDL::Init(media))
		{
			SDL::perror("SDL::Init");
			return EXIT_FAILURE;
		}

		// Allow selection of video driver back-end from command line

		if (not video.empty() and SDL_VideoInit(video.c_str()))
		{
			SDL::perror(video);
			return EXIT_FAILURE;
		}

		// Allow redirection of input from file given on command line

		auto file = stdin;
		if (not input.empty())
		{
			auto path = input.c_str();
			file = std::fopen(path, "r");
			if (not file)
			{
				SDL::perror(path, std::strerror(errno));
				return EXIT_FAILURE;
			}
		}

		// Initialize the standard input stream handler

		if (InitCommand(file, buffer, strict))
		{
			SDL::perror("InitCommand");
			return EXIT_FAILURE;
		}
	}
	SDL::ProcessEvents();
	return EXIT_SUCCESS;
}

