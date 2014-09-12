#ifndef __Tree__
#define __Tree__

#include <vector>
#include <queue>
#include "Plane.hpp"
#include "Mesh.hpp"

const int Leaf = Knot;

struct Branch
{
	int face, front, back;
};

struct Tree : virtual Mesh
{
	std::vector<Branch> nodes;
	std::vector<Plane> spaces;

	double iris;
	int root;

	std::vector<int> view;
	std::queue<int> visit;
	
	bool Traverse(Vector eye, Vector at);
	void Perspective(double fov, double aspect);

	int AddNode(Branch node);

	bool Save(const char *path);
	bool Load(const char *path);
};

#endif // file


