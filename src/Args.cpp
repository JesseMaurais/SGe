#include "Args.hpp"
#include "Strings.hpp"
#include <getopt.h>

CommandLineOption ParseCommandLine(int argc, char **argv)
{
	const struct option options[] =
	{
	{ CommandLine::Help    , no_argument       , nullptr , Option::Help    },
	{ CommandLine::Configs , required_argument , nullptr , Option::Configs },
	{ CommandLine::Media   , optional_argument , nullptr , Option::Media   },
	{ CommandLine::Quit    , no_argument       , nullptr , Option::Quit    },
	{ nullptr              , 0                 , nullptr , Option::End     }
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

}