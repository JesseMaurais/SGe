#ifndef __Bezier__
#define __Bezier__

#include "Vector.hpp"

struct Bezier
{
	Vector Ax, Ay, By, Bx;

	void Cone(Vector A, Vector Z, Vector B);
	void Cube(Vector A, Vector X, Vector Y, Vector B);

	Vector Equate(double u);
	Vector Tangent(double u);
	
	// Smoothing, control point calculators

	static Vector Smoothing(Vector A, Vector B);
	static Vector Cone2Cube(Vector A, Vector B);
	static Vector Cube2Cone(Vector A, Vector B);
	static Vector Cone2Cube(Vector A, Vector B, Vector C);
	static Vector Cube2Cone(Vector A, Vector B, Vector C);
};

#endif // file
