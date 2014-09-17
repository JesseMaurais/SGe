#ifndef __Model__
#define __Model__

#include "Mesh.hpp"
#include "Nurbs.hpp"
#include "Tessel.hpp"

struct Model : virtual Nurbs, Tessel
{
	void Cube(Scalar size);
};

#endif // file

