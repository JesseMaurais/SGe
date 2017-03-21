#include "Ply.hpp"
#include <cstdio>
#include <rply.h>


static int Vertex(p_ply_argument arg)
{
	ModelPly *self;
	long axis;
	ply_get_argument_user_data(arg, (void**) &self, &axis);
	long index;
	if (ply_get_argument_element(arg, nullptr, &index))
	{
		self->vertexes[index].v[axis] = ply_get_argument_value(arg);
	}
	return 1;
}

static int Face(p_ply_argument arg)
{
	ModelPly *self;
	long index;
	ply_get_argument_user_data(arg, (void**)&self, &index);
	long length, point;
	ply_get_argument_property(arg, NULL, &length, &point);

	static std::vector<int> data;

	if (point < 0)
	 data.resize(length);
	else
	 data[point] = (int) ply_get_argument_value(arg);
	
	if (point+1 == length)
	{
		self->BeginPolygon();
		for (const long point : data)
		{
			self->Next(point);
		}
		self->EndPolygon();
	}
	return 1;
}

static void Error(p_ply ply, const char *message)
{
	std::fprintf(stderr, "%s\n", message);
}

bool ModelPly::Load(const char *path)
{
	p_ply ply = ply_open(path, ::Error, 0, this);

	if (ply && ply_read_header(ply))
	{
	 int nVertexes =
	  ply_set_read_cb(ply, "vertex", "x", ::Vertex, this, 0);
	  ply_set_read_cb(ply, "vertex", "y", ::Vertex, this, 1);
	  ply_set_read_cb(ply, "vertex", "z", ::Vertex, this, 2);

	 int nFaces =
	  ply_set_read_cb(ply, "face", "vertex_indices", ::Face, this, 0);

	 vertexes.resize(nVertexes);
	 faces.reserve(nFaces);

	 return ply_read(ply);
	} else return false;
}

