#ifndef __Stroke__
#define __Stroke__

#include "Bezier.hpp"
#include "Tessel.hpp"

struct Stroke : virtual Tessel, Bezier
{
	Stroke(double size=5);
	void Point(double size);
	void Curve(double n);
	void Move(Vector to);
	void Line(Vector to);
	void Cone(Vector X, Vector to);
	void Cube(Vector X, Vector Y, Vector to);

 protected:

	double step;
	Vector from;
	bool moving;
};

#endif // file

