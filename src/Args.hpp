#ifndef Args_hpp
#define Args_hpp

enum class Option : char
{
	Unknown  = '?',
	Help     = 'h',
	Configs  = 'f',
	Media    = 'm',
	End      = '\0'
};

struct CommandLineOption
{
	enum Option opt;
	const char *arg;
};

CommandLineOption ParseCommandLine(int argc, char **argv);
void PrintCommandLineOptions();

#endif // file
