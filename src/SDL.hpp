#ifndef SDL_hpp
#define SDL_hpp

#include <SDL2/SDL.h>

extern SDL_Event *Event;
extern SDL_MessageBoxColorScheme MessageBoxColorScheme;

signed SDL_perror(const char *prefix);

template <unsigned Media> struct ScopedSubSystem
{
	ScopedSubSystem()
	{
		if (SDL_InitSubSystem(Media))
		{
			SDL_perror("SDL_InitSubSystem");
		}
	}

	~ScopedSubSystem()
	{
		SDL_QuitSubSystem(Media);
	}
};

#endif // file

