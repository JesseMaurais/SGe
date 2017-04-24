#include "Command.hpp"
#include "Strings.hpp"
#include "Event.hpp"
#include "SDL.hpp"
#include "Lua.hpp"

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <readline/readline.h>
#include <readline/history.h>

static const char *Prompt = nullptr;
static bool Quit = false;

static SDL_Thread *Thread;
static SDL_mutex *Mutex;
static SDL_cond *Cond;

static int CommandThread(void *unused)
{
	(void) unused;
	SDL_Event event;
	Sint32 sequence = 0;
	event.type = UserEvent(ExecuteCommand);
	while (not Quit)
	{
		auto line = readline(Prompt);
		event.user.code = ++sequence;
		event.user.data1 = line;
		// Check that we have input or user quits
		if (not line or not SDL_strcasecmp(line, "Quit"))
		{
			event.type = SDL_QUIT;
			Quit = true;
		}
		// Send the event, wait on processing
		if (not SDL_PushEvent(&event))
		{
			SDL_perror("SDL_PushEvent");
		}
		else
		if (not Quit)
		{
			if (SDL_LockMutex(Mutex))
			{
				SDL_perror("SDL_LockMutex");
			}
			if (SDL_CondWait(Cond, Mutex))
			{
				SDL_perror("SDL_CondWait");
			}
			if (SDL_UnlockMutex(Mutex))
			{
				SDL_perror("SDL_UnlockMutex");
			}
		}
	}
	return EXIT_SUCCESS;
}

bool CommandExec(const SDL_Event &event)
{
	if (SDL_LockMutex(Mutex))
	{
		SDL_perror("SDL_LockMutex");
	}

	union {
		char *string;
		void *address;
	};
	address = event.user.data1;
	int sequence = event.user.code;
	if (luaL_dostring(CoState, string))
	{
		char message[64];
		std::sprintf(message, "[%d]", sequence);
		Lua_perror(CoState, message);
	}
	add_history(string);
	std::free(address);

	if (SDL_CondSignal(Cond))
	{
		SDL_perror("SDL_CondSignal");
	}
	if (SDL_UnlockMutex(Mutex))
	{
		SDL_perror("SDL_UnlockMutex");
	}
	return true;
}

signed CommandInit(const char *prompt)
{
	Cond = SDL_CreateCond();
	if (not Cond)
	{
	 SDL_perror("SDL_CreateCond");
	 return SetError(CannotCreateCommandQueue);
	}
	Mutex = SDL_CreateMutex();
	if (not Mutex)
	{
	 SDL_DestroyCond(Cond);
	 SDL_perror("SDL_CreateMutex");
	 return SetError(CannotCreateCommandQueue);
	}
	Thread = SDL_CreateThread(CommandThread, "Command", nullptr);
	if (not Thread)
	{
	 SDL_DestroyCond(Cond);
	 SDL_DestroyMutex(Mutex);
	 SDL_perror("SDL_CreateThread");
	 return SetError(CannotCreateCommandQueue);
	}
	PushEventHandler(UserEvent(ExecuteCommand), CommandExec);
	Prompt = prompt;
	return 0;
}

void CommandQuit()
{
	PopEventHandler(UserEvent(ExecuteCommand));
	Quit = true;
	SDL_WaitThread(Thread, nullptr);
	SDL_DestroyMutex(Mutex);
	SDL_DestroyCond(Cond);
}
