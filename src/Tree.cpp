#include "Metric.hpp"
#include "Tree.hpp"
#include <fstream>
#include <cstdio>
#include <cmath>
#include "io.hpp"
#include "numeric.hpp"

bool Tree::Traverse(Vector const &eye, Vector const &at)
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
			visible.push_back(node.face);
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
	return to_int(index);
}

bool Tree::Save(std::string const& path)
{
	std::ofstream file(path);
	if (not file)
	{
		return false;
	}
	if (not io::fprintf(file, "nodes={1}\n", nodes.size()))
	{
		return false;
	}
	for (Branch &node : nodes)
	{
		if (not io::fprintf(file, "{1} {2} {3}\n", node.face, node.front, node.back))
		{
			return false;
		}
	}
	return true;
}

bool Tree::Load(std::string const& path)
{
	std::ifstream file(path);
	if (not file)
	{
		return false;
	}
	std::string line;
	if (not std::getline(file, line))
	{
		return false;
	}
	auto const header = fmt::key_value(line);
	if (header.first != "nodes")
	{
		return false;
	}
	auto const size = to<std::size_t>(header.second);
	nodes.reserve(size);
	while (std::getline(file, line))
	{
		Branch node;
		std::stringstream stream(line);
		stream >> node.face;
		stream >> node.front;
		stream >> node.back;
		nodes.push_back(node);
	}
	return nodes.size() == size;
}
