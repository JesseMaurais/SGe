#ifndef __Plane__
#define __Plane__

#include "Number.hpp"
#include "Vector.hpp"

struct Plane
{
	enum Crossing { Planar, Spanning, Front, Back };

	Vector normal;
	Scalar distance;

	Plane();
	Plane(Vector N, Vector V);
	Scalar Dot(Vector V);
	Scalar Equate(Vector V);
	Scalar Cut(Vector U, Vector V);
	bool Agree(Vector N);
	Crossing Classify(Vector V[], int n=3);
};

#endif // file
