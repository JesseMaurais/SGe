#ifndef __Tree__
#define __Tree__

#include "Plane.hpp"
#include "Mesh.hpp"
#include <vector>
#include <queue>

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
	void Perspective(Scalar fov, Scalar aspect);

	int AddNode(Branch node);

	bool Save(const char *path);
	bool Load(const char *path);
};

#endif // file


