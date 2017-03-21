#ifndef Model_hpp
#define Model_hpp

#include "Mesh.hpp"
#include "Nurbs.hpp"
#include "Tessel.hpp"

struct Model : virtual Nurbs, Tessel
{
	void Cube(Scalar size);
};

#endif // file

