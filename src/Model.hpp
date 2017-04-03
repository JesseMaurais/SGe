#ifndef Model_hpp
#define Model_hpp

#include "Mesh.hpp"
#include "Nurbs.hpp"
#include "Tessel.hpp"
#include "Stroke.hpp"

struct Model : virtual Mesh, Nurbs, Tessel, Stroke
{
	void Cube(Scalar size);
};

#endif // file

