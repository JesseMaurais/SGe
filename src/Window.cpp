#include "Window.hpp"
#include "Collection.hpp"
#include "Strings.hpp"
#include "SDL.hpp"
#include <typeinfo>

using WindowSet = Collection<Window>;
const auto WindowName = typeid(Window).name();

Window::Window(char const *title, int x, int y, int w, int h, unsigned mode)
{
	window = SDL_CreateWindow(title, x, y, w, h, mode);
	if (window)
	{
		SDL_SetWindowData(window, WindowName, this);
	}
	else
	{
		SDL_perror("SDL_CreateWindow");
	}
	SDL_verify(WindowSet::Instance().Add(this));
}

Window::~Window()
{
	SDL_verify(WindowSet::Instance().Remove(this));
	SDL_verify(this == SDL_SetWindowData(window, WindowName, nullptr));
	SDL_DestroyWindow(window);
}
