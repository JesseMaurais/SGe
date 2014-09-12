#include "SDL.hpp"
#include "OpenGL.hpp"

SDL_Event *Event;
SDL_Window *Window;

const Uint32 USER_EVENT = SDL_RegisterEvents(LAST_EVENT);

void SDL_perror(const char *string)
{
	SDL_Log("%s: %s", string, SDL_GetError());
}
	
static signed FindFormat(SDL_PixelFormat *pixmap, GLenum *format, GLenum *type)
{
	switch (pixmap->format)
	{
	 case SDL_PIXELFORMAT_RGB332:
		*format = GL_RGB;
		*type = GL_UNSIGNED_BYTE_3_3_2;
		break;
	 case SDL_PIXELFORMAT_RGB444:
		*format = GL_RGB;
		*type = GL_UNSIGNED_SHORT_4_4_4_4; // ?
		break;
	 case SDL_PIXELFORMAT_RGB555:
		*format = GL_RGB;
		*type = GL_UNSIGNED_SHORT_1_5_5_5_REV; // ?
		break;
	 case SDL_PIXELFORMAT_BGR555:
		*format = GL_BGR;
		*type = GL_UNSIGNED_SHORT_1_5_5_5_REV; // ?
		break;
	 case SDL_PIXELFORMAT_ARGB4444:
		*format = GL_BGRA;
		*type = GL_UNSIGNED_SHORT_4_4_4_4_REV;
		break;
	 case SDL_PIXELFORMAT_RGBA4444:
		*format = GL_RGBA;
		*type = GL_UNSIGNED_SHORT_4_4_4_4;
		break;
	 case SDL_PIXELFORMAT_ABGR4444:
		*format = GL_BGRA;
		*type = GL_UNSIGNED_SHORT_4_4_4_4_REV;
		break;
	 case SDL_PIXELFORMAT_BGRA4444:
		*format = GL_BGRA;
		*type = GL_UNSIGNED_SHORT_4_4_4_4;
		break;
	 case SDL_PIXELFORMAT_ARGB1555:
		*format = GL_BGRA;
		*type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
		break;
	 case SDL_PIXELFORMAT_RGBA5551:
		*format = GL_RGBA;
		*type = GL_UNSIGNED_SHORT_5_5_5_1;
		break;
	 case SDL_PIXELFORMAT_ABGR1555:
		*format = GL_RGBA;
		*type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
		break;
	 case SDL_PIXELFORMAT_BGRA5551:
		*format = GL_BGRA;
		*type = GL_UNSIGNED_SHORT_5_5_5_1;
		break;
	 case SDL_PIXELFORMAT_RGB565:
		*format = GL_RGB;
		*type = GL_UNSIGNED_SHORT_5_6_5;
		break;
	 case SDL_PIXELFORMAT_BGR565:
		*format = GL_BGR;
		*type = GL_UNSIGNED_SHORT_5_6_5;
		break;
	 case SDL_PIXELFORMAT_RGB24:
		*format = GL_RGB;
		*type = GL_UNSIGNED_INT; // ?
		break;
	 case SDL_PIXELFORMAT_BGR24:
		*format = GL_BGR;
		*type = GL_UNSIGNED_INT; // ?
		break;
	 case SDL_PIXELFORMAT_RGB888:
		*format = GL_RGB;
		*type = GL_UNSIGNED_INT_8_8_8_8; // ?
		break;
	 case SDL_PIXELFORMAT_RGBX8888:
		*format = GL_RGB;
		*type = GL_UNSIGNED_INT_8_8_8_8; // ?
		break;
	 case SDL_PIXELFORMAT_BGR888:
		*format = GL_BGR;
		*type = GL_UNSIGNED_INT_8_8_8_8; // ?
		break;
	 case SDL_PIXELFORMAT_BGRX8888:
		*format = GL_BGR;
		*type = GL_UNSIGNED_INT_8_8_8_8; // ?
		break;
	 case SDL_PIXELFORMAT_ARGB8888:
		*format = GL_BGRA;
		*type = GL_UNSIGNED_INT_8_8_8_8_REV;
		break;
	 case SDL_PIXELFORMAT_RGBA8888:
		*format = GL_RGBA;
		*type = GL_UNSIGNED_INT_8_8_8_8;
		break;
	 case SDL_PIXELFORMAT_ABGR8888:
		*format = GL_RGBA;
		*type = GL_UNSIGNED_INT_8_8_8_8_REV;
		break;
	 case SDL_PIXELFORMAT_BGRA8888:
		*format = GL_BGRA;
		*type = GL_UNSIGNED_INT_8_8_8_8;
		break;
	 case SDL_PIXELFORMAT_ARGB2101010:
		*format = GL_BGRA;
		*type = GL_UNSIGNED_INT_2_10_10_10_REV; // ?
		break;
	 default:
		return -1;
	}
	return 0;
}

SDL_Surface *LoadSurface(const char *path, int format)
{
	SDL_Surface *image = IMG_Load(path);
	if (!image)
	{
	 SDL_perror("IMG_Load");
	 return NULL;
	}
	SDL_Surface *convert = SDL_ConvertSurfaceFormat(image, format, 0);
	SDL_FreeSurface(image);
	if (!convert)
	{
	 SDL_perror("SDL_ConvertSurfaceFormat");
	 return NULL;
	}
	return convert;
}

signed LoadTexture(SDL_Surface *surface, int level, int internal)
{
	GLenum format, type;
	if (FindFormat(surface->format, &format, &type) < 0)
	{
	 return SDL_SetError("cannot find matching format");
	}
	glTexImage2D
	( GL_TEXTURE_2D
	, level
	, internal
	, surface->w
	, surface->h
	, 0
	, format
	, type
	, surface->pixels
	);
	GLenum error = glGetError();
	if (error)
	{
	 return SDL_SetError("glTexImage2D: %s", gluErrorString(error));
	}
	return 0;
}

signed LoadMipmaps(SDL_Surface *surface, int internal)
{
	GLenum format, type;
	if (FindFormat(surface->format, &format, &type) < 0)
	{
	 return SDL_SetError("cannot find matching format");
	}
	GLint error = gluBuild2DMipmaps
	( GL_TEXTURE_2D
	, internal
	, surface->w
	, surface->h
	, format
	, type
	, surface->pixels
	);
	if (error)
	{
	 return SDL_SetError("gluBuild2DMipmaps: %s", gluErrorString(error));
	}
	return 0;
}

signed LoadCubeMap(SDL_Surface *surface, int face, int internal)
{
	GLenum format, type;
	if (FindFormat(surface->format, &format, &type) < 0)
	{
	 return SDL_SetError("cannot find matching format");
	}
	glTexImage2D
	( face
	, 0
	, internal
	, surface->w
	, surface->h
	, 0
	, format
	, type
	, surface->pixels
	);
	GLenum error = glGetError();
	if (error)
	{
	 return SDL_SetError("glTexImage2D: %s", gluErrorString(error));
	}
	return 0;
}

bool HaveEvent(enum UserEventType type)
{
	return Event->type == type+USER_EVENT;
}

signed PushEvent(enum UserEventType type, int code, void *d, void *p)
{
	SDL_Event event;
	//event.user.timestamp = SDL_GetTicks();
	//event.user.windowID = SDL_GetWindowID(Window);
	event.user.type = type+USER_EVENT;
	event.user.code = code;
	event.user.data1 = d;
	event.user.data2 = p;
	if (SDL_PushEvent(&event) < 0)
	{
	 SDL_perror("SDL_PushEvent");
	 return SDL_SetError("cannot queue user event");
	}
	return 0;
}

