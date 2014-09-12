#ifndef __Quaternion__
#define __Quaternion__

#include "Vector.hpp"

struct Quaternion
{
	union {
	 double v[4];
	 struct {
	  double w, x, y, z;
	 };
	};
	Quaternion(void);
	void Concatenate(Quaternion Q, Quaternion R);
	void Rotate(double radian, double x, double y, double z);
	double Normalize(void);
	void Convert(double matrix []);
	Vector Direction(void);
};

#endif // file
