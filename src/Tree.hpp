#ifndef Tree_hpp
#define Tree_hpp

#include <vector>
#include <queue>
#include <string>
#include "Number.hpp"
#include "Plane.hpp"
#include "Mesh.hpp"


struct Tree : virtual Mesh
{
	static constexpr int Leaf = Knot;

	struct Branch
	{
		int face, front, back;
	};

	std::vector<Branch> nodes;
	std::vector<Plane> spaces;

	Scalar iris;
	int root;

	std::vector<int> view;
	std::queue<int> visit;
	
	bool Traverse(Vector const &eye, Vector const &at);
	void Perspective(Scalar fov, Scalar aspect);

	int AddNode(Branch node);

	bool Save(std::string const& path);
	bool Load(std::string const& path);
};

#endif // file


