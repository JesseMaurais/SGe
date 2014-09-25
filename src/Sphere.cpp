#include "Sphere.hpp"
#include "Hedron.hpp"

void Sphere::AddEdge(Edge E)
{
	int index = edges.size();
	edges.push_back(E);
	return index;
}

void Sphere::AddAdjacent(Adjacent A)
{
	int index = adjacents.size();
	adjacents.push_back(A);
	return index;
}

void Sphere::Create(int depth)
{
	for (int i = 0; i < icos::nRects; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			AddVertex(icos::Rects[i][j]);
		}
	}
	for (int i = 0; i < icos::nEdges; ++i)
	{
		Edge E;
		for (int j = 0; j < 2; ++j)
		{
			E.child[j] = Knot;
		}
		E.point = Knot;
		AddEdge(E);
	}
	for (int i = 0; i < icos::nFaces; ++i)
	{
		Surface S:
		Adjacent A:
		for (int j = 0; j < 3; ++j)
		{
			S.points[j] = icos::PFaces[i][j];
			A.edges[j] = icos::EFaces[i].edges[j];
		}
		for (int j = 0; j < 4; ++j)
		{
			A.child[j] = Knot;
		}
		AddAdjacent(A);
		AddSurface(S);
	}
}

void Sphere::Divide()
{
	int n = edges.size();
	edges.reserve(3*n);
	n = faces.size();

	for (int i = 0; i < n; ++i)
	{
		Surface &S = faces[i];
		Adjacent &A = adjacents[i];

		Surface T;
		Adjacent B[4];

		for (int j = 0, k = 2; j < 3; k = j++)
		{
			Edge &F = edges[A.edges[j]];
			if (Knot == F.point)
			{
			 F.point = NewVertex(S.points[j], S.points[k], 0.5);
			 Edge E = {Knot, Knot, Knot};
			 F.child[0] = AddEdge(E);
			 F.child[1] = AddEdge(E);
			}
			T.points[j] = F.point;
		}
	}
	return 
}




