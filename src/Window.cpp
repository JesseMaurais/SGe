#include "Window.hpp"
#include "Error.hpp"
#include "SDL.hpp"
#include <SDL2/SDL_syswm.h>
#include <set>


static std::set<SDL_Window*> WindowSet;


SDL_Window *CreateWindow(const char *title, int x, int y, int w, int h, unsigned flags, SDL_Window *parent)
{
	SDL_Window *window = nullptr;
	SDL_SysWMinfo syswm;

	if (parent and SDL_GetWindowWMInfo(parent, &syswm))
	{
		switch (syswm.subsystem)
		{
		#if SDL_VIDEO_DRIVER_X11
		case SDL_SYSWM_X11:
		{
			const unsigned long pixel = XBlackPixel(syswm.info.x11.display, XDefaultScreen(syswm.info.x11.display));
			Window id = XCreateSimpleWindow(syswm.info.x11.display, syswm.info.x11.window, x, y, w, h, 0, pixel, pixel);
			window = SDL_CreateWindowFrom(reinterpret_cast<void*>(id));
		}
		break;
		#endif

		default:
			SDL_assert(not "Cannot match window system manager");

		}
		SDL_assert(window and "Failed to create child window");
	}

	if (not window)
	{
		window = SDL_CreateWindow(title, x, y, w, h, flags);
	}

	if (window)
	{
		auto pair = WindowSet.insert(window);
		SDL_assert(pair.second and "Created window was not unique");
	}

	return window;
}

void DestroyWindow(SDL_Window *window)
{
	SDL_DestroyWindow(window);
	std::size_t count = WindowSet.erase(window);
	SDL_assert(count > 0 and "Window was not in set");
	SDL_assert(count >= 1 and "Window was not unique in set");
}

unsigned GetWindowCount()
{
	std::size_t size = WindowSet.size();
	return static_cast<unsigned>(size);
}

bool HaveOpenWindows()
{
	return not WindowSet.empty();
}
