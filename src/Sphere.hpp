#ifndef __Sphere__
#define __Sphere__

#include "Model.hpp"
#include <vector>

struct Edge
{
	int point, child[2];
};

struct Adjacent
{
	int edges[3], child[4];
};

struct Sphere
{
	std::vector<Edge> edges;
	std::vector<Adjacent> adjacents;

	int AddEdge(Edge);
	int AddAdjacent(Adjacent);

	void Create(int depth);
};

#endif // file

