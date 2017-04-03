#ifndef Window_hpp
#define Window_hpp

struct SDL_Window;

class Window
{
public:

	Window(char const *title, int x, int y, int w, int h, unsigned mode);
	virtual ~Window();

protected:

	SDL_Window *window;
};

#endif // file
