#ifndef BSP_hpp
#define BSP_hpp

#include "Model.hpp"
#include "Tree.hpp"
#include <vector>

struct BSP : virtual Model, Tree
{
	struct Polygon : std::vector<int>
	{
		int face;
	};

	struct Brush : std::vector<Polygon>
	{
		bool operator!()
		{
			return empty();
		}
	};

	int Sort();
	int Sort(Brush &brush);
	int Sort(Brush &brush, Brush &front, Brush &back);
	int Select(Brush &brush);
	void Split(Polygon &poly, Plane &space, Brush &front, Brush &back);
	void Split(Polygon &poly, Plane &space, Polygon &front, Polygon &back);
	Plane::Crossing Classify(Plane &space, Polygon &poly);
};

#endif // file

