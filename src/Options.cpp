#include "Options.hpp"
#include "Strings.hpp"
#include <getopt.h>
#include <cstdio>

struct option options[] =
{
	// Command line arguments
	{CommandLineOption::Help, no_argument, nullptr, Option::Help},
	{CommandLineOption::Configs, required_argument, nullptr, Option::Configs},
	{CommandLineOption::Video, no_argument, nullptr, Option::Video},
	{CommandLineOption::Audio, no_argument, nullptr, Option::Audio},
	// Terminus
	{0, 0, 0, 0}
};

static void PrintUsage()
{

}

CommandLine ParseCommandLine(int &argc, char **argv)
{
	CommandLine cmd;
	int index = 0;
	const int opt = getopt_long_only(argc, argv, "", options, &index);
	switch (opt)
	{
	case -1:
		cmd.opt = Option::End;
		break;
	case '?':
		std::printf(CommandLineOption::Unknown, argv[optind]);
		// no break
	case Option::Help:
		cmd.opt = Option::Help;
		PrintUsage();
		break;
	}
	return cmd;
}

