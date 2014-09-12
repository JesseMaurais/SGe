#ifndef __VBO__
#define __VBO__

#include "Mesh.hpp"
#include "OpenGL.hpp"

struct VBO : virtual Mesh
{
	union {
	 GLuint buffers[4];
	 struct {
	  GLuint element, vertex, normal, texCoord;
	 };
	};
	size_t size;
	void Enable();
	void Disable();
	void GenBuffers(GLenum usage);
	void DeleteBuffers();
	void DrawBuffers();
};

#endif // file
