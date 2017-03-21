#ifndef __VBO__
#define __VBO__

#include "Mesh.hpp"

struct VBO : virtual Mesh
{
	union {
	 unsigned buffers[5];
	 struct {
	  unsigned element, vertex, texCoord, normal, color;
	 };
	};
	void GenBuffers(unsigned usage);
	void DeleteBuffers();
	void Enable();
	void Disable();
	void BindBuffers();
	void Draw(int group);
	void Draw();
};

#endif // file

