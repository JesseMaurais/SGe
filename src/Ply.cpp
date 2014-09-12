#include "Ply.hpp"
#include <cstdio>

bool ModelPly::Load(const char *path)
{
	p_ply ply = ply_open(path, error, 0, this);
	
	if (ply && ply_read_header(ply))
	{
	 int nVertexes =
	  ply_set_read_cb(ply, "vertex", "x", vertex, this, 0);
	  ply_set_read_cb(ply, "vertex", "y", vertex, this, 1);
	  ply_set_read_cb(ply, "vertex", "z", vertex, this, 2);
	
	 int nFaces =
	  ply_set_read_cb(ply, "face", "vertex_indices", face, this, 0);
	
	 vertexes.resize(nVertexes);
	 faces.reserve(nFaces);
	
	 return ply_read(ply);
	} else return false;
}

void ModelPly::Vertex(p_ply_argument arg, int axis)
{
	long index;
	if (ply_get_argument_element(arg, NULL, &index))
	{
	 vertexes[index].v[axis] = ply_get_argument_value(arg);
	}
}

void ModelPly::Face(p_ply_argument arg, int index)
{
	long length, point;
	ply_get_argument_property(arg, NULL, &length, &point);

	static std::vector<int> data;

	if (point < 0)
	 data.resize(length);
	else
	 data[point] = (int) ply_get_argument_value(arg);
	
	if (point+1 == length)
	{
	 Begin(GL_POLYGON);
	  for (point = 0; point < data.size(); ++point)
	  {
	   Next(data[point]);
	  }
	 End();
	}
}

int ModelPly::vertex(p_ply_argument arg)
{
	ModelPly *self;
	long index;
	ply_get_argument_user_data(arg, (void**)&self, &index);
	self->Vertex(arg, index);
	return 1;
}

int ModelPly::face(p_ply_argument arg)
{
	ModelPly *self;
	long index;
	ply_get_argument_user_data(arg, (void**)&self, &index);
	self->Face(arg, index);
	return 1;
}

void ModelPly::error(p_ply ply, const char *message)
{
	fprintf(stderr, "%s\n", message);
}

