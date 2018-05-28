#include <SDL2/SDL_config.h>
#include "Pipe.hpp"
#include "System.hpp"
#include "SDL.hpp"
#include <cstdio>

SDL_RWops *SDL::RWFromProcess(char const *command, char const *mode)
{
	if constexpr (POSIX)
	{
		FILE *fp = popen(command, mode);
		if (fp)
		{
			SDL_RWops *ops = SDL_RWFromFP(fp, SDL_TRUE);
			if (ops)
			{
				ops->close = [](SDL_RWops *ops)->int
				{
					return pclose(ops->hidden.stdio.fp);
				};
				return ops;
			}
			fclose(fp);
		}
		return nullptr;
	}
	else
	if constexpr (WIN32)
	{
		SECURITY_ATTRIBUTES sa;
		PROCESS_INFORMATION pi;
		STARTUPINFO si;
		HANDLE input;
		HANDLE output;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
		sa.nLength = sizeof(sa);
		sa.bInheritHandle = TRUE;
		sa.lpSecurityDescriptor = NULL;

		if (not CreatePipe(&si.hStdInput, &input, &sa, 0))
		{
			return nullptr;
		}
		if (not SetHandleInformation(si.hStdInput, HANDLE_FLAG_INHERIT, 0))
		{
			return nullptr;
		}
		if (not CreatePipe(&si.hStdOutput, &output, &sa, 0))
		{
			return nullptr;
		}
		if (not SetHandleInformation(si.hStdOutput, HANDLE_FLAG_INHERIT, 0))
		{
			return nullptr;
		}

		si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

		BOOL const ok = CreateProcess
		(
		 NULL,    // no application name
		 command, // command line
		 NULL,    // process security attributes
		 NULL,    // primary thread security attributes
		 FALSE,   // handles are not inherited
		 0,       // creation flags
		 NULL,    // use parent's environment
		 NULL,    // use parent's current directory
		 &si,     // startup information in
		 &pi      // process information out
		);

		if (not ok)
		{
			return nullptr;
		}

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}

namespace sys::io
{
	Pipe::Pipe(SDL_RWops *rwops) : ops(rwops)
	{}

	Pipe::~Pipe()
	{
	    SDL_FreeRW(ops);
	}
}
