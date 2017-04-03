#ifndef Stroke_hpp
#define Stroke_hpp

#include "Number.hpp"
#include "Bezier.hpp"
#include "Tessel.hpp"

struct Stroke : virtual Tessel, Bezier
{
	Stroke(Scalar step=5);
	void Steps(Scalar s);
	void Curve(Scalar n);
	void Move(Vector const &to);
	void Line(Vector const &to);
	void Cone(Vector const &X, Vector const &to);
	void Cube(Vector const &X, Vector const &Y, Vector const &to);

 protected:

	Scalar step;
	Vector from;
	bool moving;
};

#endif // file

