#include "Surface.hpp"
#include "Strings.hpp"
#include "OpenGL.hpp"
#include "SDL.hpp"

#include <SDL2/SDL_image.h>


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
		return SDL_SetError(String(CannotFindFormat));
	}
	return 0;
}

SDL_Surface *LoadSurface(const char *path, unsigned format)
{
	SDL_Surface *image = IMG_Load(path);
	if (not image)
	{
		SDL::LogError("IMG_Load");
		return nullptr;
	}
	SDL_Surface *convert = SDL_ConvertSurfaceFormat(image, format, 0);
	SDL_FreeSurface(image);
	if (not convert)
	{
		SDL::LogError("SDL_ConvertSurfaceFormat");
		return nullptr;
	}
	return convert;
}

signed LoadMipmaps(SDL_Surface *surface, unsigned internal)
{
	GLenum format, type;
	if (FindFormat(surface->format, &format, &type) < 0)
	{
		return SDL::LogError("FindFormat");
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
	return OpenGL::SetError("gluBuild2DMipmaps", error);
}

signed LoadTexture(SDL_Surface *surface, unsigned level, unsigned internal)
{
	GLenum format, type;
	if (FindFormat(surface->format, &format, &type) < 0)
	{
		return SDL::LogError("FindFormat");
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
	return OpenGL::CheckError("glTexImage2D");
}

signed LoadCubeMap(SDL_Surface *surface, unsigned face, unsigned internal)
{
	GLenum format, type;
	if (FindFormat(surface->format, &format, &type) < 0)
	{
		return SDL::LogError("FindFormat");
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
	return OpenGL::CheckError("glTexImage2D");
}
