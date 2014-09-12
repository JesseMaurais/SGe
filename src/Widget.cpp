#include "GUI.hpp"
#include "SDL.hpp"
#include "Event.hpp"
#include <cstring>
#include <cstdarg>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

struct Strings
{
	const char **param, **value;

	Strings(const char **attributes)
	{
		value = attributes;
		param = value++;
	}

	bool operator!()
	{
		return *param && *value;
	}

	void operator++()
	{
		param = ++value;
		value++;
	}

	bool operator()(const char *string)
	{
		return !SDL_strcasecmp(*param, string);
	}

	const char *operator()()
	{
		return *value;
	}

	int scan(const char *format, ...)
	{
		va_list va;
		va_start(va, format);
		int n = vsscanf(*value, format, va);
		va_end(va);
		return n;
	}
};

struct Surface : GUI
{
	SDL_Surface *surface;

	Surface()
	{
		surface = NULL;
	}

	~Surface()
	{
		if (surface) SDL_FreeSurface(surface);
	}

	void Draw()
	{
		//if (surface) SDL_BlitSurface(surface, NULL, Screen, this);
	}
};

struct Image : Surface
{
	Image(const char **attributes) : Surface()
	{
		for (Strings arg(attributes); !arg; ++arg)
		{
			if (arg("path"))
			{
				surface = IMG_Load(arg());
				if (!surface)
					SDL_perror("IMG_Load");

				w = surface->w;
				h = surface->h;
			}
		}
	}
};

struct Label : Surface
{
	Label(const char **attributes) : Surface()
	{
		const char *path = "data/Serif.ttf";
		const char *text = "Hello World!";
		SDL_Color color = {255,255,255};
		unsigned int pointSize = 10;

		for (Strings arg(attributes); !arg; ++arg)
		{
			if (arg("font"))
				path = arg();
			else
			if (arg("text"))
				text = arg();
			else
			if (arg("pt"))
				arg.scan("%u", &pointSize);
			else
			if (arg("color"))
			{
				unsigned int mask;
				if (arg.scan("#%x", &mask))
				{
				 color.a = (mask & 0xFF000000) >> 24 ;
				 color.r = (mask & 0x00FF0000) >> 16 ;
				 color.g = (mask & 0x0000FF00) >> 8  ;
				 color.b = (mask & 0x000000FF)       ;
				}
			}
		}

		TTF_Font *font = TTF_OpenFont(path, pointSize);
		if (!font)
		{
		 SDL_perror("TTF_OpenFont");
		 return;
		}
		surface = TTF_RenderText_Blended(font, text, color);
		TTF_CloseFont(font);
		if (!surface)
		{
		 SDL_perror("TTF_RenderText_Blended");
		 return;
		}

		w = surface->w;
		h = surface->h;
	}
};

static struct RegistryClass
{
	RegistryClass(void)
	{
		Register<Image>("Image");
		Register<Label>("Label");
	}

} Registry;

