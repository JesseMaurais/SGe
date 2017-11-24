#include "JavaScript.hpp"
#include "Stream.hpp"
#include "Error.hpp"
#include "Event.hpp"
#include "SDL.hpp"
#include "std.hpp"
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
			Path      = '*', // Path to this program
			InputFile = 'f', // Name of a file to use for input stream
			Buffer    = 'S', // Size of the buffer used for input stream
			Strict    = 's', // Be strict with script
			ByteCode  = 'b', // Dump script byte code to output stream
			Video     = 'v', // Enable video with optional named back-end
			Help      = 'h', // Print command line help
			Quit      = 'q', // Quit immediately after setup
			Unknown   = '?', // Unknown command line option
			End       = '\0' // End of command line options

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
		Option opt;
		int arg = getopt_long_only(argc, argv, "+", options, nullptr);
		switch (arg)
		{
		case -1:
			opt.param = Option::End;
			opt.value = nullptr;
			break;
		case '?':
			opt.param = 0 == optind ? Option::Path : Option::Unknown;
			opt.value = argv[optind];
			break;
		default:
			opt.param = decltype(opt.param)(arg);
			opt.value = optarg;
		}
		return opt;
	}

	void PrintCommandLineOptions()
	{
		constexpr auto arg = "ARG";

		for (struct option const &opt : options)
		{
			if (not opt.name)
			{
				return; // list is null terminated
			}

			// Give option name and indicate argument style

			std::string style;
			switch (opt.has_arg)
			{
			case no_argument:
				stl::sprintf(style, "-%1 or --%2", (char) opt.val, opt.name);
				break;

			case optional_argument:
				stl::sprintf(style, "-%1 [%3] or --%2[=%3]", (char) opt.val, opt.name, arg);
				break;

			case required_argument:
				stl::sprintf(style, "-%1 %3 or --%2=%3", (char) opt.val, opt.name, arg);
				break;
			}

			// Give explanation of option

			std::string line;
			switch (opt.val)
			{
			case Option::InputFile:
				line = "Stream from file instead of standard input";
				break;

			case Option::Buffer:
				line = "Size of buffer used to read from input stream";
				break;

			case Option::Strict:
				line = "Be strict with script";
				break;

			case Option::Video:
				line = "Enable video with optional named back-end";
				break;

			case Option::Help:
				line = "Show this help message";
				break;

			case Option::Quit:
				line = "Quit immediately after startup";
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
			auto opt = NextOption(argc, argv);
			switch (opt.param)
			{
			case Option::Path:
				// program name
				continue;

			case Option::End:
				done = true;
				continue;

			case Option::InputFile:
				input = opt.value;
				continue;

			case Option::Buffer:
				buffer = std::stoi(opt.value);
				continue;

			case Option::Strict:
				strict = true;
				continue;

			case Option::ByteCode:
				jerry = JERRY_INIT_SHOW_OPCODES;
				continue;

			case Option::Video:
				media |= SDL_INIT_VIDEO;
				video = opt.value;
				continue;

			case Option::Quit:
				SDL::SendUserEvent(EscapeEvent);
				continue;

			case Option::Unknown:
				SDL::Log(InvalidArgument, opt.value);
				// no break

			case Option::Help:
				PrintCommandLineOptions();
				return EXIT_SUCCESS;
			}
		}

		// Bind our interfaces to the script engine

		if (not js::Init(jerry))
		{
			SDL::perror("Init Engine");
			return EXIT_FAILURE;
		}

		// Startup the minimum specified media back-ends

		if (not SDL::Init(media))
		{
			SDL::perror("Init Media");
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

		// Initialize the input stream thread

		if (InitStream(file, buffer, strict))
		{
			SDL::perror("Init Stream");
			return EXIT_FAILURE;
		}
	}
	SDL::ProcessEvents();
	return EXIT_SUCCESS;
}

