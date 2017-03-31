#include "Window.hpp"
#include "Collect.hpp"
#include "SDL.hpp"

using WindowSet = Collect<Window>;

Window::Window()
{
	SDL_verify(WindowSet::Instance().Add(this));
}

Window::~Window()
{
	SDL_verify(WindowSet::Instance().Remove(this));
}
