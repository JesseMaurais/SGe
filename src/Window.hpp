#ifndef Window_hpp
#define Window_hpp

#include <SDL2/SDL_syswm.h>

namespace sys::wm
{
    struct Info : SDL_SysWMinfo
    {
        Info(SDL_Window* window);

        SDL_Window *Sub(SDL_Rect const &area);

        operator bool() const
        {
        	return target != nullptr;
        }

    private:

    	SDL_Window *target;
    };
}

#endif // file
