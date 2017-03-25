#ifndef SDL_hpp
#define SDL_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

signed SDL_perror(const char *origin, const char *error);
signed SDL_perror(const char *origin);

void SetAssertHandler(SDL_Window *window);
void ResetAssertHandler();

#endif // file

