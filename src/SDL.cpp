#include "SDL.hpp"

static SDL_Event InternEvent;
SDL_Event *Event = &InternEvent;

SDL_MessageBoxColorScheme MessageBoxColorScheme =
{
	{
		{  47,  52,  63 }, // SDL_MESSAGEBOX_COLOR_BACKGROUND
		{ 211, 218, 227 }, // SDL_MESSAGEBOX_COLOR_TEXT
		{  56,  60,  74 }, // SDL_MESSAGEBOX_COLOR_BUTTON_BORDER
		{  64,  69,  82 }, // SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND
		{  64, 132, 214 }  // SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED
	}
};

signed SDL_perror(const char *prefix)
{
	const char *error = SDL_GetError();
	if (error)
	{
		SDL_Log("%s: %s", prefix, error);
		return 0;
	}
	return -1;
}
