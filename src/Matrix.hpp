#ifndef Matrix_hpp
#define Matrix_hpp

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
	void Concatenate(Matrix const &M, Matrix const &N);
	void Translate(Scalar const x, Scalar const y, Scalar const z);
	void Scale(Scalar x, Scalar const y, Scalar const z);
	void Rotate(Scalar radian, Scalar x, Scalar y, Scalar z);
	Vector Direction() const;
	Vector Position() const;
	void Transpose();
	Vector Transform(Vector const &V);
	void Tangent(Vector const V [3], Vector const C [3]);
};

#endif // file
