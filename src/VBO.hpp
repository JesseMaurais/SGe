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
	void GenBuffers(GLenum usage);
	void DeleteBuffers();
	void Enable();
	void Disable();
	void BindBuffers();
	void Draw(int group);
	void Draw();
};

#endif // file

