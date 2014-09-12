#ifndef __Plane__
#define __Plane__

#include "Vector.hpp"

struct Plane
{
	enum Cross { Planar, Spanning, Front, Back };

	Vector normal;
	double distance;

	Plane(void);
	Plane(Vector N, Vector V);
	double Dot(Vector V);
	double Equate(Vector V);
	double Cut(Vector U, Vector V);
	bool Agree(Vector N);
	Cross Classify(Vector V[], int n=3);
};

#endif // file
