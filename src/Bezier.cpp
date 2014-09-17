#include "Bezier.hpp"

void Bezier::Cone(Vector A, Vector Z, Vector B)
{
	Vector X = (A + Z * 2.0) / 3.0;
	Vector Y = (B + Z * 2.0) / 3.0;

	Cube(A, X, Y, B);
}

void Bezier::Cube(Vector A, Vector X, Vector Y, Vector B)
{
	Ax = A; Ay = X; By = Y; Bx = B;
}

Vector Bezier::Equate(Scalar u)
{
	Scalar v = 1.0-u, v2 = v*v, v3 = v2*v, u2 = u*u, u3 = u2*u;
	return Ax*v3 + Ay*v2*u*3.0 + By*u2*v*3.0 + Bx*u3;
}

Vector Bezier::Tangent(Scalar u)
{
	Scalar v = 1.0-u, v2 = v*v, u2 = u*u;
	return Ax*v2*3.0 + Ay*v*6.0 + By*u*6.0 + Bx*u2*3.0;
}
	
// Smoothing, control point calculators

Vector Bezier::Smoothing(Vector A, Vector B)
{
	return A*2.0 - B;
}

Vector Bezier::Cone2Cube(Vector A, Vector B)
{
	return (A*5.0 - B*2.0) / 3.0;
}

Vector Bezier::Cube2Cone(Vector A, Vector B)
{
	return (A*5.0 - B*3.0) / 2.0;
}

Vector Bezier::Cone2Cube(Vector A, Vector B, Vector C)
{
	return (A*8.0 - B*6.0 + C) / 3.0;
}

Vector Bezier::Cube2Cone(Vector A, Vector B, Vector C)
{
	return (A - B)*4.0 + C;
}

