#include "Sphere.hpp"
#include "Hedron.hpp"
#include <cassert>

int Sphere::AddNode(Surface N)
{
	int index = nodes.size();
	nodes.push_back(N);
	return index;
}

int Sphere::NewVertex(Edge E)
{
	Vector &A = GetVertex(E.points[0]);
	Vector &B = GetVertex(E.points[1]);
	return AddVertex((A + B)/2.0);
}

Sphere::Sphere(int depth)
{
	faces.reserve(icos::nFaces);
	edges.reserve(icos::nEdges);
	nodes.reserve(icos::nFaces);

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
			E.points[j] = icos::Edges[i].points[j];
		}
		AddEdge(E);
	}

	BeginGroup(0); // depth = 0
	for (int i = 0; i < icos::nFaces; ++i)
	{
		Surface N, S;
		for (int j = 0; j < 3; ++j)
		{
			S.points[j] = icos::PFaces[i][j];
			N.edges[j] = icos::EFaces[i].edges[j];
		}
		AddSurface(S);
		AddNode(N);
	}
	EndGroup();

	for (int i = 0; i < depth; ++i)
	{
		Divide(i);
	}

	edges.clear();
	nodes.clear();
}

void Sphere::Divide(int depth)
{
	// Calculate new vertex in each edge

	int nEdges = edges.size();
	int indexes[nEdges];
	for (int i = 0; i < nEdges; ++i)
	{
		indexes[i] = NewVertex(edges[i]);
	}

	// Divide the faces into a new group

	Group & group = groups[depth];
	BeginGroup(++depth);
	for (int i = 0; i < group.count; ++i)
	{
		Surface R, S;
		int edge, face = group.first + i;
		for (int j = 0, k = 2; j < 3; k = j++)
		{
			S.points[0] = faces[face].points[j];
			edge = nodes[i].edges[j];
			S.points[1] = indexes[edge];
			edge = nodes[i].edges[k];
			S.points[2] = indexes[edge];
			AddSurface(S);
			R.points[j] = edge;
		}
		AddSurface(R);
	}
	EndGroup();

	// Divide each edge by the new vertex
	{
		int splits[nEdges][2];
		std::vector<Edge> prior;
		prior.swap(edges);
		for (int i = 0; i < prior.size(); ++i)
		{
			Edge E, F;
			E.points[1] = F.points[0] = indexes[i];
			E.points[0] = prior[i].points[0];
			F.points[1] = prior[i].points[1];
			splits[i][0] = AddEdge(E);
			splits[i][1] = AddEdge(F);
		}
	}

	// Create the nodes and interior edges
	{
		int splits[nEdges][2];
		std::vector<Surface> prior;
		prior.swap(nodes);
		for (int i = 0; i < prior.size(); ++i)
		{
			Edge E;
			Surface R, S;
			int edge;
			for (int j = 0, k = 2; j < 3; k = j++)
			{
				edge = prior[i].edges[k];
				E.points[0] = indexes[edge];
				edge = prior[i].edges[j];
				E.points[1] = indexes[edge];
				edge = AddEdge(E);
				R.edges[j] = edge;
				S.edges[0] = edge;
				edge = nodes[i].edges[k];
				S.edges[1] = splits[edge][0];
				edge = nodes[i].edges[j];
				S.edges[2] = splits[edge][1];
				AddNode(S);
			}
			AddNode(R);
		}
	}
}

