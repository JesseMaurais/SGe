#include "Args.hpp"
#include "Strings.hpp"
#include <getopt.h>

CommandLineOption ParseCommandLine(int argc, char **argv)
{
	const struct option options[] =
	{
	{ String(CmdHelp)    , no_argument       , nullptr , Option::Help    },
	{ String(CmdConfigs) , required_argument , nullptr , Option::Configs },
	{ String(CmdMedia)   , optional_argument , nullptr , Option::Media   },
	{ String(CmdQuit)    , no_argument       , nullptr , Option::Quit    },
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
