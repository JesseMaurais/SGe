#ifndef __Model__
#define __Model__

#include "Mesh.hpp"
#include "Nurbs.hpp"
#include "Tessel.hpp"

struct Model : virtual Nurbs, Tessel
{
	void Cube(Scalar size);
	void GeoSphere(Scalar radius, int depth);

 protected:

	void Divide(Scalar radius, int depth, Vector V [3]);
};

#endif // file

