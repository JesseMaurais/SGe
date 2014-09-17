#ifndef __Stroke__
#define __Stroke__

#include "Number.hpp"
#include "Bezier.hpp"
#include "Tessel.hpp"

struct Stroke : virtual Tessel, Bezier
{
	Stroke(Scalar step=5);
	void Steps(Scalar s);
	void Curve(Scalar n);
	void Move(Vector to);
	void Line(Vector to);
	void Cone(Vector X, Vector to);
	void Cube(Vector X, Vector Y, Vector to);

 protected:

	Scalar step;
	Vector from;
	bool moving;
};

#endif // file

