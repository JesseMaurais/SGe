#ifndef __SDL__
#define __SDL__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define SDL_ShowError(message)\
 SDL_ShowSimpleMessageBox(SDL_MESSAGE_BOX_ERROR, __func__, message, Window)

void SDL_perror(const char *string);

extern SDL_Event *Event;
extern SDL_Window *Window;

SDL_Surface *LoadSurface(const char *path, int format);
signed LoadMipmaps(SDL_Surface *surface, int internal);
signed LoadTexture(SDL_Surface *surface, int internal, int level);
signed LoadCubeMap(SDL_Surface *surface, int internal, int face);

extern const Uint32 USER_EVENT;
enum UserEventType
{
 UPDATE,
 LAST_EVENT
};

bool HaveEvent(enum UserEventType);
signed PushEvent(enum UserEventType, int code=0, void *d=NULL, void *p=NULL);

#endif // file

