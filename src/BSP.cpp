#include "BSP.hpp"
#include <cmath>
#include <vector>
#include <climits>
#include <cstdlib>

int BSP::Sort()
{
	int n = faces.size();
	Polygon polygon;	
	Brush brush;

	nodes.reserve(n);
	spaces.resize(n);
	
	for (polygon.face = 0; polygon.face < n; ++polygon.face)
	{
		spaces[polygon.face] = GetPlane(polygon.face);

		for (int it = 0; it < 3; ++it)
		{
		 	int point = faces[polygon.face].points[it];
			if (point < 0) point = points[~point].vertex;
			polygon.push_back(point);
		}

		brush.push_back(polygon);
		polygon.clear();
	}

	return Sort(brush);
}

int BSP::Sort(Brush &brush)
{
	if (not brush) return Leaf;

	Branch node;
	Brush front, back;

	std::size_t n = vertexes.size();
	node.face = Sort(brush, front, back);
	brush.clear();
	node.front = Sort(front);
	node.back = Sort(back);
	vertexes.resize(n);

	return AddNode(node);
}

int BSP::Sort(Brush &brush, Brush &front, Brush &back)
{
	std::size_t size = brush.size();
	int const divider = Select(brush);
	Plane &space = spaces[divider];

	for (std::size_t it = 0; it < size; ++it)
	{
		Polygon &polygon = brush[it];

		if (divider == polygon.face) continue;

		switch (Classify(space, polygon))
		{
		case Plane::Spanning:
			Split(polygon, space, front, back);
			break;
		case Plane::Planar:
			if (spaces[polygon.face].Agree(space.normal))
			{
				front.push_back(polygon);
			}
			else
			{
				back.push_back(polygon);
			}
			break;
		case Plane::Front:
			front.push_back(polygon);
			break;
		case Plane::Back:
			back.push_back(polygon);
			break;
		}
	}

	return divider;
}

int BSP::Select(Brush &brush)
{
	std::size_t items = brush.size();
	int bestDivider, bestScore = INT_MAX;

	for (std::size_t it = 0; it < items; ++it)
	{
		int divider = brush[it].face;
		Plane &space = spaces[divider];
		int spanning=0, front=0, back=0;

		for (std::size_t test = 0; test < items; ++test)
		{
			if (it == test) continue;

			Polygon &polygon = brush[test];

			switch (Classify(space, polygon))
			{
			case Plane::Spanning:
				++spanning;
				break;
			case Plane::Planar:
				if (spaces[polygon.face].Agree(space.normal))
				{
					++front;
				}
				else
				{
					++back;
				}
				break;
			case Plane::Front:
				++front;
				break;
			case Plane::Back:
				++back;
				break;
			}
		}

		int score = std::abs(front - back) + spanning;

		if (score < bestScore)
		{
			bestDivider = divider;
			bestScore = score;
		}

		if (not bestScore) break;
	}

	return bestDivider;
}

void BSP::Split(Polygon &polygon, Plane &space, Brush &frontSet, Brush &backSet)
{
	Polygon front, back;
	Split(polygon, space, front, back);
	frontSet.push_back(front);
	backSet.push_back(back);
}

void BSP::Split(Polygon &polygon, Plane &space, Polygon &front, Polygon &back)
{
	front.face = back.face = polygon.face;

	int a, b;
	Vector A, B;
	double dotA, dotB;
	std::size_t n = polygon.size();

	a = polygon[n-1];
	A = vertexes[a];
	dotA = space.Equate(A);

	for (std::size_t it = 0; it < n; ++it, a=b, A=B, dotA=dotB)
	{
		b = polygon[it];
		B = vertexes[b];
		dotB = space.Equate(B);

		if (dotB > 0)
		{
			if (dotA < 0)
			{
				double cut = space.Cut(A, B);
				int c = NewVertex(a, b, cut);

				front.push_back(c);
				back.push_back(c);
			}
			front.push_back(b);
		}
		else
		if (dotB < 0)
		{
			if (dotA > 0)
			{
				double cut = space.Cut(A, B);
				int c = NewVertex(a, b, cut);

				front.push_back(c);
				back.push_back(c);
			}
			back.push_back(b);
		}
		else
		{
			front.push_back(b);
			back.push_back(b);
		}
	}
}

Plane::Crossing BSP::Classify(Plane &space, Polygon &polygon)
{
	int *indexes = polygon.data();
	std::size_t size = polygon.size();
	std::vector<Vector> vertexes(size);
	Vector *pointer = vertexes.data();
	GetVertexes(indexes, pointer, size);
	return space.Classify(pointer, size);
}

