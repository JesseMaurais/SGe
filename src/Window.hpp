#ifndef Window_hpp
#define Window_hpp

struct SDL_Window;
/// Create a window with optional parent and add it to the window set
SDL_Window *CreateWindow(const char *title, int x, int y, int w, int h, unsigned flags, SDL_Window *parent=nullptr);
/// Remove from the window set and destroy
void DestroyWindow(SDL_Window *window);
/// Number of windows in the set
unsigned GetWindowCount();
/// Window set is not empty
bool HaveOpenWindows();

#endif // file
