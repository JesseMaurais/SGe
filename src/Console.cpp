#include "Console.hpp"
#include "Strings.hpp"
#include "Event.hpp"
#include "Error.hpp"
#include "SDL.hpp"

#include <future>
#include <cstdio>
#include <cstdlib>
#include <readline/readline.h>
#include <readline/history.h>

namespace
{
	void Thread(char const *prompt)
	{
		bool done = false;

		while (not done)
		{
			auto line = readline(prompt);
			if (not line)
			{
				break;
			}
			add_history(line);

			SDL::SendUserEvent(ExecuteCommand, 0, line);
		}
	}
}

bool InitConsole(char const *prompt)
{
	std::async(Thread, prompt);
}

