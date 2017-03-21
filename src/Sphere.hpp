#ifndef __Sphere__
#define __Sphere__

#include "Mesh.hpp"
#include <vector>

struct Sphere : MeshComposer
{
	Sphere(int depth);

 private:

	void Divide(int depth);

	std::vector<Surface> nodes;

	int AddNode(Surface);
	int NewVertex(Edge);
};

#endif // file

