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
			Prompt    = 'p', // Feed lines from console into input stream
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

	struct option const Options[] =
	{
		{ "prompt"   , optional_argument , nullptr , Option::Prompt    },
		{ "file"     , optional_argument , nullptr , Option::InputFile },
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
		int arg = getopt_long_only(argc, argv, "+", Options, nullptr);
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

	void PrintCommandLineOptions(std::string const &programName)
	{
		stl::printf("%1 a scene graph engine for soft geometry\n", programName);

		for (struct option const &opt : Options)
		{
			if (not opt.name)
			{
				return; // list is null terminated
			}

			// Give explanation of option

			std::string arg = "ARG";
			std::string line;
			switch (opt.val)
			{
			case Option::Prompt:
				arg = "TEXT";
				line = "Customize the text used for command prompt";
				break;

			case Option::InputFile:
				arg = "FILE";
				line = "Stream from named file or standard input if no file name is given";
				break;

			case Option::Buffer:
				arg = "SIZE";
				line = "Size of buffer used to read from input stream";
				break;

			case Option::Strict:
				line = "Be strict with script";
				break;

			case Option::Video:
				arg = "DRIVER";
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

			// Print formatted line to stdout

			stl::printf("\n%1\n\t%2\n", style, line);
		}
	}
}

int main(int argc, char **argv)
{
	SDL::ScopedAssertHandler handler;
	{
		std::string programName = stl::filesystem::path(argv[0]).filename().string();

		// Defaults for the JavaScript engine

		jerry_init_flag_t jerry = JERRY_INIT_EMPTY;
		bool strict = false;

		// Defaults for the media back-end

		Uint32 media = SDL_INIT_EVENTS;
		std::string video;

		// Defaults for the input stream

		std::string prompt = String(CommandPrompt);
		std::size_t buffer = BUFSIZ;
		std::FILE *file = nullptr;

		// Parse command line options

		bool done = argc < 2;
		while (not done)
		{
			auto opt = NextOption(argc, argv);
			switch (opt.param)
			{
			case Option::End:
				done = true;
				continue;

			case Option::Path:
				// program name
				continue;

			case Option::Prompt:
				prompt = opt.value;
				continue;

			case Option::InputFile:
				file = opt.value ? std::fopen(opt.value, "r") : stdin;
				if (not file)
				{
					SDL::LogError(opt.value, std::strerror(errno));
					return EXIT_FAILURE;
				}
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
				PrintCommandLineOptions(programName);
				return EXIT_SUCCESS;
			}
		}

		// Bind our interfaces to the script engine

		if (not js::Init(jerry))
		{
			SDL::LogError("Init Engine");
			return EXIT_FAILURE;
		}

		// Startup the minimum specified media back-ends

		if (not SDL::Init(media))
		{
			SDL::LogError("Init Media");
			return EXIT_FAILURE;
		}

		// Allow selection of video driver back-end from command line

		if (not video.empty() and SDL_VideoInit(video.c_str()))
		{
			SDL::LogError(video);
			return EXIT_FAILURE;
		}

		// Initialize the input stream thread

		if (not InitStream(prompt, file, buffer, strict))
		{
			SDL::LogError("Init Stream");
			return EXIT_FAILURE;
		}
	}
	SDL::ProcessEvents();
	return EXIT_SUCCESS;
}

