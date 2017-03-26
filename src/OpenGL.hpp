/** \file
 * Utilities for working with OpenGL: error string report and graphics context
 * singleton for resource management.
 */

#ifndef OpenGL_hpp
#define OpenGL_hpp

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

struct SDL_Window;

/// Set the SDL error string to the given error code
signed OpenGL_SetError(const char *origin, GLenum error);
/// Set the SDL error string to the current error code
signed OpenGL_SetError(const char *origin);
/// Log the current error with SDL
signed OpenGL_LogError(const char *origin);

/**
 * \brief Update the graphics device singleton for compatibility with the given window.
 * \param window Window for which the singleton context will be made compatible.
 * Provide null to delete the context. It will be deleted automatically at exit.
 * \return Context handle now current, or null and the error string is set.
 */
void *OpenGL_GetContext(SDL_Window *window);

#endif // file

