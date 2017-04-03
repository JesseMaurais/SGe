#include "Metric.hpp"
#include "Tree.hpp"
#include "STL.hpp"
#include <cstdio>
#include <cmath>

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
		if (Leaf != node.front)
		{
			visit.push(node.front);
		}

		if (space.Dot(at) < iris)
		{
			if (Leaf != node.back)
			{
				visit.push(node.back);
			}
			view.push_back(node.face);
		}
	}
	else
	{
		if (Leaf != node.back)
		{
			visit.push(node.back);
		}

		if (space.Dot(at) > iris)
		{
			if (Leaf != node.front)
			{
				visit.push(node.front);
			}
		}
	}

	visit.pop();
	return true;
}

void Tree::Perspective(Scalar fov, Scalar aspect)
{
	Vector frame;

	frame.x = Radian * fov/2.0;
	fov /= aspect;
	frame.y = Radian * fov/2.0;

	fov = frame.Magnitude();
	iris = std::sin(fov);
}

int Tree::AddNode(Branch node)
{
	std::size_t index = nodes.size();
	nodes.push_back(node);
	return stl::as_int(index);
}

bool Tree::Save(char const *path)
{
	auto file = std::fopen(path, "w");
	if (not file)
	{
		std::perror(path);
		return false;
	}

	std::size_t size = nodes.size();
	if (std::fprintf(file, "nodes=%d\n", stl::as_int(size)) < 0)
	{
		std::fputs("Error writing to stream", stderr);
	}

	for (std::size_t it = 0; it < size; ++it)
	{
		Branch const &node = nodes[it];
		std::fprintf(file, "%d %d %d\n", node.face, node.front, node.back);
	}

	if (std::fclose(file))
	{
		std::perror(path);
	}
	return true;
}

bool Tree::Load(char const *path)
{
	auto file = std::fopen(path, "r");
	if (not file)
	{
		std::perror(path);
		return false;
	}

	int size;
	switch (std::fscanf(file, "nodes=%d\n", &size))
	{
	case 1:
		break;
	case std::EOF:
		if (std::ferror(file))
		{
			std::perror(path);
		}
		else
		if (std::feof(file))
		{
			std::fputs("Unexpected end of file", stderr);
		}
		// no break
	default:
		if (std::fclose(file))
		{
			std::perror(path);
		}
		return false;
	}

	nodes.resize(size);

	for (std::size_t it = 0; it < size; ++it)
	{
		Branch &N = nodes[it];

		switch (std::fscanf(file, "%d %d %d\n", &N.face, &N.front, &N.back))
		{
		case 3:
			break;
		case std::EOF:
			if (std::ferror(file))
			{
				std::perror(path);
			}
			else
			if (std::feof(file))
			{
				std::fputs("Unexpected end of file", stderr);
			}
			// no break
		default:
			if (std::fclose(file))
			{
				std::perror(path);
			}
			return false;
		}
	}
	
	if (std::fclose(file))
	{
		std::perror(path);
	}
	return true;
}

