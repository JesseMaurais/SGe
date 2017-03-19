#ifndef __OpenGL__
#define __OpenGL__

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

signed OpenGL_SetError(const char *string, GLenum error);
signed OpenGL_CheckError(const char *string);

#endif // file

