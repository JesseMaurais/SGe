#ifndef __Render__
#define __Render__

#include "Mesh.hpp"

struct Render : virtual Mesh
{
	void DrawGroup(Group);
	void Draw();
};

#endif // file

