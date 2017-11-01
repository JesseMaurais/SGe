/** \file
 * Utilities for working with OpenGL:
 * 		Reporting of errors through SDL
 *		Graphics context singleton for resource management.
 */

#ifndef OpenGL_hpp
#define OpenGL_hpp

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

struct SDL_Window;

namespace OpenGL
{
	/// Set the SDL error string to the given error code
	signed SetError(const char *origin, GLenum error);
	/// Set the SDL error string to the current error code
	signed CheckError(const char *origin);
	/// Log the current error with SDL
	signed LogError(const char *origin);

	// Texture id at the given index from within the singleton.
	GLuint GetTexture(unsigned index);

	/**
	 * \brief Update the graphics device singleton for compatibility with the
	 * given window.
	 * \param window Window for which the singleton context will be made
	 * compatible. Provide null to delete the context. It will be deleted
	 * automatically at exit.
	 * \return Context handle now current, or null and the error string is set.
	 */
	void *GetContext(SDL_Window *window);
}

#endif // file

