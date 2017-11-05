#include "Args.hpp"
#include "Strings.hpp"
#include <getopt.h>

CommandLineOption ParseCommandLine(int argc, char **argv)
{
	const struct option options[] =
	{
	{ "help"    , no_argument       , nullptr , Option::Help    },
	{ "configs" , required_argument , nullptr , Option::Configs },
	{ "media"   , optional_argument , nullptr , Option::Media   },
	{ "quit"    , no_argument       , nullptr , Option::Quit    },
	{ nullptr         , 0                 , nullptr , Option::End     }
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
