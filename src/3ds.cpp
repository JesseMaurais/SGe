#include "3ds.hpp"
#include <vector>
#include <cstring>

bool Model3ds::Load(const char *path)
{
	Lib3dsFile *file = lib3ds_file_load(path);
	if (file)
	{
		Lib3dsNode *node = file->nodes;
		while (node)
		{
			AddNode(file, node);
			node = node->next;
		}
		lib3ds_file_free(file);
	}
	return file != NULL;
}

void Model3ds::AddChildNodes(Lib3dsFile *file, Lib3dsNode *node)
{
	for (node = node->childs; node; node = node->next)
	{
		AddNode(file, node);
	}
}

void Model3ds::AddNode(Lib3dsFile *file, Lib3dsNode *node)
{
	if (!strcmp(node->name, "$$$DUMMY")) return;

	switch (node->type)
	{
	 case LIB3DS_OBJECT_NODE:
	 {
	  Lib3dsMesh *mesh = lib3ds_file_mesh_by_name(file, node->name);
	  AddMesh(file, mesh);
	 }
	 break;
	}
	AddChildNodes(file, node);
}

void Model3ds::AddMesh(Lib3dsFile *file, Lib3dsMesh *mesh)
{
	std::vector<int> indexes;

	for (int i = 0; i < mesh->points; ++i)
	{
		Lib3dsPoint *point = mesh->pointL + i;
	
		Vector V;
	
		V.x = point->pos[0];
		V.y = point->pos[2];
		V.z = point->pos[1];

		int index = AddVertex(V);
		indexes.push_back(index);
	}
	
	for (int i = 0; i < mesh->faces; ++i)
	{
		Lib3dsFace *face = mesh->faceL + i;

		Surface S;
	
		S.points[2] = indexes.at(face->points[0]);
		S.points[1] = indexes.at(face->points[1]);
		S.points[0] = indexes.at(face->points[2]);
	
		AddSurface(S);
	}
}


