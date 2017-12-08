#ifndef Quaternion_hpp
#define Quaternion_hpp

#include "Number.hpp"
#include "Vector.hpp"

struct Quaternion
{
	Scalar w, x, y, z;

	Quaternion();
	void Concatenate(Quaternion const &Q, Quaternion const &R);
	void Rotate(Scalar radian, Scalar x, Scalar y, Scalar z);
	Scalar Normalize();
	void Convert(Scalar matrix []) const;
	Vector Direction() const;
};

#endif // file
