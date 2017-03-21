#ifndef __Quaternion__
#define __Quaternion__

#include "Number.hpp"
#include "Vector.hpp"

struct Quaternion
{
	union {
	 Scalar v[4];
	 struct {
	  Scalar w, x, y, z;
	 };
	};
	Quaternion();
	void Concatenate(Quaternion Q, Quaternion R);
	void Rotate(Scalar radian, Scalar x, Scalar y, Scalar z);
	Scalar Normalize();
	void Convert(Scalar matrix []);
	Vector Direction();
};

#endif // file
