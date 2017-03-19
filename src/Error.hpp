#ifndef SGe_Error
#define SGe_Error

struct SDL_Window;
void SetErrorHandler(SDL_Window *window);
void ResetErrorHandler();

#endif // file
