#include "Model.hpp"
#include <cassert>
#include <cstdio>

void Model::Begin(int mode)
{
	flag = (GL_POLYGON == mode);
	
	if (flag)
	 Tessel::BeginPolygon();
	else
	 MeshComposer::Begin(mode);
}

void Model::Next(int index)
{
	if (flag)
	{
	 Vector V = Mesh::GetVertex(index);
	 Tessel::Vertex(index, V.v);
	}
	else
	 MeshComposer::Next(index);
}

void Model::End()
{
	if (flag)
	 Tessel::EndPolygon();
	else
	 MeshComposer::End();
}

void Model::Vertex(double x, double y, double z)
{
	Vector vertex(x, y, z);
	int index = Mesh::AddVertex(vertex);
	MeshComposer::Next(index);
}

void Model::TexCoord(double x, double y, double z)
{
	Mesh::AddTexCoord(Vector(x, y, z));
}

void Model::Normal(double x, double y, double z)
{
	Mesh::AddNormal(Vector(x, y, z));
}


