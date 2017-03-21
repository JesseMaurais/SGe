#ifndef Options_hpp
#define Options_hpp

enum class Option : char
{
	Help    = 'h',
	Configs = 'c',
	Video   = 'v',
	Audio   = 'a',
	End     =  0
};

struct CommandLine
{
	enum Option opt;
	const char *arg;

} ParseCommandLine(int &argc, char **argv);

void PrintCommandLineHelp(CommandLine);

#endif // file
