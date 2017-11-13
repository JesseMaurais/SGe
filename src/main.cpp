#include "Desktop.hpp"
#include "Command.hpp"
#include "Strings.hpp"
#include "Event.hpp"
#include "Args.hpp"
#include "Lua.hpp"
#include "SDL.hpp"
#include <cstdlib>

static bool Quit = false;
static bool OnQuit(const SDL_Event &event)
{
	SDL_assert(SDL_QUIT == event.type);
	Quit = true;
	return true;
}

int main(int argc, char **argv)
{
	std::vector<std::string> paths;
	if (xdg::OpenFile(paths, xdg::OpenFile::Multiple))
	{
		for (auto const &path : paths)
		{
			std::cout << path << std::endl;
		}
	}
	else
	{
		std::cout << "failed" << std::endl;
	}
	return 0;

	SDL::PushEventHandler(SDL_QUIT, OnQuit);
	SDL::SetAssertionHandler(nullptr);
	{
		unsigned media = SDL_INIT_EVENTS;
		char const *script = nullptr;

		if (argc > 1)
		{
			CommandLineOption cmd;
			do {
				cmd = ParseCommandLine(argc, argv);
				switch (cmd.opt)
				{
				case Option::Quit:
					Quit = true;
					break;
				case Option::Media:
					media |= SDL_INIT_EVERYTHING;
					break;
				case Option::Configs:
					script = cmd.arg;
					break;
				case Option::Help:
					return EXIT_SUCCESS;
				}
			}
			while (Option::End != cmd.opt);
		}

		if (SDL_Init(media))
		{
			SDL::perror("SDL_Init");
			return EXIT_FAILURE;
		}
		else
		if (std::atexit(SDL_Quit))
		{
			SDL_Log(String(CannotMakeExit), "SDL_Quit");
			return EXIT_FAILURE;
		}

		if (not Lua::Init(script))
		{
			SDL::perror("Lua::Init");
			return EXIT_FAILURE;
		}
		else
		if (std::atexit(Lua::Quit))
		{
			SDL_Log(String(CannotMakeExit), "Lua_Quit");
			return EXIT_FAILURE;
		}
	}

	SDL_Event event;
	while (not Quit)
	{
		if (SDL_WaitEvent(&event))
		{
			SDL::Dispatch(event);
		}
		else
		{
			SDL::perror("SDL_WaitEvent");
		}
	}
	return EXIT_SUCCESS;
}

