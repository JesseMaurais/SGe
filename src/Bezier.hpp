#ifndef Bezier_hpp
#define Bezier_hpp

#include "Number.hpp"
#include "Vector.hpp"

struct Bezier
{
	Vector Ax, Ay, By, Bx;

	void Cone(Vector const &A, Vector const &Z, Vector const &B);
	void Cube(Vector const &A, Vector const &X, Vector const &Y, Vector const &B);

	Vector Equate(Scalar u);
	Vector Tangent(Scalar u);
	
	// Smoothing, control point calculators

	static Vector Smoothing(Vector const &A, Vector const &B);
	static Vector Cone2Cube(Vector const &A, Vector const &B);
	static Vector Cube2Cone(Vector const &A, Vector const &B);
	static Vector Cone2Cube(Vector const &A, Vector const &B, Vector const &C);
	static Vector Cube2Cone(Vector const &A, Vector const &B, Vector const &C);
};

#endif // file
