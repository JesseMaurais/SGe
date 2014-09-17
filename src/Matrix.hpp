#ifndef __Matrix__
#define __Matrix__

#include "Number.hpp"
#include "Vector.hpp"

struct Matrix
{
	union {
	 Scalar dim[4][4], v[16];
	 struct {
	  struct { Scalar x, y, z, w; } X, Y, Z, W;
	 };
	};

	Matrix();
	void Concatenate(Matrix M, Matrix N);
	void Translate(Scalar x, Scalar y, Scalar z);
	void Scale(Scalar x, Scalar y, Scalar z);
	void Rotate(Scalar radian, Scalar x, Scalar y, Scalar z);
	Vector Direction();
	Vector Position();
	void Transpose();
	Vector Transform(Vector V);
	void Tangent(Vector V [3], Vector C [3]);
};

#endif // file
