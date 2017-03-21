#ifndef Model3ds_hpp
#define Model3ds_hpp

#include "Model.hpp"

#include <lib3ds/file.h>
#include <lib3ds/node.h>
#include <lib3ds/mesh.h>

struct Model3ds : virtual Model
{
	bool Load(const char *path);

 private:

	void AddChildNodes(Lib3dsFile *file, Lib3dsNode *node);
	void AddNode(Lib3dsFile *file, Lib3dsNode *node);
	void AddMesh(Lib3dsFile *file, Lib3dsMesh *mesh);
};

#endif // file

