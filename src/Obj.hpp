#ifndef __Obj__
#define __Obj__

#include "Model.hpp"

struct ModelObj : virtual Model
{
	bool Load(const char *path);

 private:

	int ReadVertex(char *line);
	int ReadTexCoord(char *line);
	int ReadNormal(char *line);
	int ReadPoint(char *it);
	int ReadFace(char *line);
};

#endif // file

