/** \file
 * Utility functions for working with OpenGL.
 */

#ifndef OpenGL_hpp
#define OpenGL_hpp

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

struct SDL_Window;

namespace OpenGL
{
	/// Set the SDL error string to the given error code. Always returns true.
	bool SetError(const char *origin, GLenum error);
	/// Set the SDL error string to the current error code. Returns true when an error exists.
	bool CheckError(const char *origin);
	/// Log the current error with SDL. Returns true when an error exists.
	bool LogError(const char *origin);

	/// Texture id at the given index from within the singleton.
	GLuint GetTexture(unsigned index);
	/// Program id at the given index from within the singleton.
	GLuint GetProgram(unsigned index);
	/// Shader id at the given index from within the singleton.
	GLuint GetShader(unsigned index);
	/// Set the shader type that will be generated on update.
	void SetShaderType(unsigned index, GLenum type);

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

