#ifndef Args_hpp
#define Args_hpp

namespace Option
{
	enum
	{
		Unknown  = '?',
		Help     = 'h',
		Quit     = 'q',
		Configs  = 'f',
		Media    = 'm',
		End      = '\0'
	};
}

struct CommandLineOption
{
	int opt;
	const char *arg;
};

CommandLineOption ParseCommandLine(int argc, char **argv);
void PrintCommandLineOptions();

#endif // file
