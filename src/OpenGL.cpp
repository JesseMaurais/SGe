#include "OpenGL.hpp"
#include "SDL.hpp"

signed OpenGL_SetError(const char *string, GLenum error)
{
	return SDL_SetError("%s: %s", string, gluErrorString(error));
}

signed OpenGL_CheckError(const char *string)
{
	GLenum error = glGetError();
	if (error)
	{
		return OpenGL_SetError(string, error);
	}
	return 0;
}
