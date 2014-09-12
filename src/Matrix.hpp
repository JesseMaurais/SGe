#ifndef __Matrix__
#define __Matrix__

#include "Vector.hpp"

struct Matrix
{
	union {
	 double dim[4][4], v[16];
	 struct {
	  struct { double x, y, z, w; } X, Y, Z, W;
	 };
	};

	Matrix(void);
	void Concatenate(Matrix M, Matrix N);
	void Translate(double x, double y, double z);
	void Scale(double x, double y, double z);
	void Rotate(double radian, double x, double y, double z);
	Vector Direction(void);
	Vector Position(void);
	void Transpose(void);
	Vector Transform(Vector V);
	void Tangent(Vector V [3], Vector C [3]);
};

#endif // file
