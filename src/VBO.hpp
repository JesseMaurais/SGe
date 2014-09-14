#ifndef __VBO__
#define __VBO__

#include "Mesh.hpp"
#include "OpenGL.hpp"

struct VBO : virtual Mesh
{
	union {
	 GLuint buffers[5];
	 struct {
	  GLuint element, vertex, texCoord, normal, color;
	 };
	};
	void Enable();
	void Disable();
	void GenBuffers(GLenum usage);
	void DeleteBuffers();
	void BindBuffers();
	void Draw(int group);
};

#endif // file

