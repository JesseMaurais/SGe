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
        PROCESS_INFORMATION pi;
        STARTUPINFO si;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        if (not CreateProcess
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
        ))
        {
            return nullptr;
        }
    }
}

sys::io::Pipe::Pipe(SDL_RWops *rwops)
: ops(rwops)
{}

sys::io::Pipe::~Pipe()
{
    SDL_FreeRW(ops);
}
