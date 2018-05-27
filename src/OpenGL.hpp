/** \file
 * Utility functions for working with OpenGL.
 */

#ifndef OpenGL_hpp
#define OpenGL_hpp

#include "Signal.hpp"
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

struct SDL_Window;

namespace OpenGL
{
	/// Set the SDL error string to the given error code. Always returns true
	bool SetError(const char *origin, GLenum error);
	/// Set the SDL error string to the current error code. Returns true when an error exists
	bool CheckError(const char *origin);
	/// Log the current error with SDL. Returns true when an error exists
	bool LogError(const char *origin);

	/**
	 * \brief Update the graphics device singleton for compatibility with the
	 * given window.
	 * \param window Window for which the singleton context will be made
	 * compatible. Provide null to return current context. It will be deleted
	 * automatically at exit.
	 * \return Context handle now current, or null and the error string is set.
	 */
	void *GetContext(SDL_Window *window = nullptr);

	// Managed resource observer slots

	struct Texture : Slot<GLuint>
	{
		Texture(Observer observer = nullptr);
	};

	struct Buffer : Slot<GLuint>
	{
		Buffer(Observer observer = nullptr);
	};

	struct Program : Slot<GLuint>
	{
		Program(Observer observer = nullptr);
	};

	struct Shader : Slot<GLuint>
	{
		Shader(GLenum type = 0, Observer observer = nullptr);
	};
}

#endif // file
