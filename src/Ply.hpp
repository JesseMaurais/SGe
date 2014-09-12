#ifndef __Ply__
#define __Ply__

#include "Model.hpp"
#include <rply.h>

struct ModelPly : virtual Model
{
	bool Load(const char *path);

 private:

	void Vertex(p_ply_argument arg, int axis);
	void Face(p_ply_argument arg, int index);

 private:

	static int vertex(p_ply_argument arg);
	static int face(p_ply_argument arg);
	static void error(p_ply ply, const char *message);
};

#endif // __Ply__

