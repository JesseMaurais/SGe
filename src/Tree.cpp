#include <cstdio>
#include <cmath>
#include "Metric.hpp"
#include "Tree.hpp"

bool Tree::Traverse(Vector eye, Vector at)
{
	if (visit.empty())
	{
	 visit.push(root);
	 return false;
	}

	int visiting = visit.front();
	Branch &node = nodes[visiting];
	Plane &space = spaces[node.face];

	if (space.Equate(eye) > 0)
	{
	 if (node.front != Leaf)
	  visit.push(node.front);

	 if (space.Dot(at) < iris)
	 {
	  if (node.back != Leaf)
	   visit.push(node.back);
	  
	  view.push_back(node.face);
	 }
	}
	else
	{
	 if (node.back != Leaf)
	  visit.push(node.back);

	 if (space.Dot(at) > iris)
	  if (node.front != Leaf)
	   visit.push(node.front);
	}

	visit.pop();
	return true;
}

void Tree::Perspective(Scalar fov, Scalar aspect)
{
	Vector frame;

	frame.x = Radian * fov/2;
	fov /= aspect;
	frame.y = Radian * fov/2;

	fov = frame.Magnitude();
	iris = sin(fov);
}

int Tree::AddNode(Branch node)
{
	int index = nodes.size();
	nodes.push_back(node);
	return index;
}

bool Tree::Save(const char *path)
{
	FILE *file = fopen(path, "w");
	if (!file)
	{
	 perror(path);
	 return false;
	}

	int size = nodes.size();
	fprintf(file, "nodes=%d\n", size);

	for (int it = 0; it < size; ++it)
	{
	 Branch &node = nodes[it];
	 fprintf(file, "%d %d %d\n", node.face, node.front, node.back);
	}

	fclose(file);
	return true;
}

bool Tree::Load(const char *path)
{
	FILE *file = fopen(path, "r");
	if (!file)
	{
	 perror(path);
	 return false;
	}

	int size;
	if (!fscanf(file, "nodes=%d\n", &size))
	{
	 fclose(file);
	 return false;
	}

	nodes.resize(size);

	for (int it = 0; it < size; ++it)
	{
		Branch &N = nodes[it];

		if (fscanf(file, "%d %d %d\n", &N.face, &N.front, &N.back) < 3)
		{
		 fclose(file);
		 return false;
		}
	}
	
	fclose(file);
	return true;
}

