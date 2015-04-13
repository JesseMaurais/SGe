#ifndef __GUI__
#define __GUI__

#include <SDL2/SDL.h>

struct GUI : SDL_Rect
{
	GUI();

	bool Load(SDL_RWops *ops);
	bool Load(const char *path);

	virtual ~GUI();
	virtual void Done();
	virtual void Draw();
	void AddChild(GUI*);
	
  protected:

	void ChildUnionRect();
	void ChildRestack();
	void ChildDraw();

  private:

	GUI *next;
	GUI *childs;
};

void Register(const char *name, GUI *(*)(const char **attributes));

template <class Widget> GUI *Constructor(const char **attributes)
{
	return new Widget(attributes);
}

template <class Widget> void Register(const char *name)
{
	Register(name, Constructor<Widget>);
}

#endif // file

