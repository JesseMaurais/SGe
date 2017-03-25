#ifndef Tree_hpp
#define Tree_hpp

#include "Plane.hpp"
#include "Mesh.hpp"
#include <vector>
#include <queue>

struct Tree : virtual Mesh
{
	static constexpr int Leaf = Knot;

	struct Branch
	{
		int face, front, back;
	};

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


