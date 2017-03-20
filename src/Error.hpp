#ifndef Error_hpp
#define Error_hpp

struct SDL_Window;
void SetErrorHandler(SDL_Window *window);
void ResetErrorHandler();

#endif // file
