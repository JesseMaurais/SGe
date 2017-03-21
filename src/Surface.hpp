#ifndef __Surface__
#define __Surface__

struct SDL_Surface;

SDL_Surface *LoadSurface(const char *path, unsigned format);
signed LoadMipmaps(SDL_Surface *surface, unsigned internal);
signed LoadTexture(SDL_Surface *surface, unsigned internal, unsigned level);
signed LoadCubeMap(SDL_Surface *surface, unsigned internal, unsigned face);

#endif // file
