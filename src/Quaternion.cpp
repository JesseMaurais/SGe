#include "Quaternion.hpp"
#include <cmath>

Quaternion::Quaternion()
{
	w = 1.0; x = y = z = 0.0;
}

void Quaternion::Concatenate(Quaternion Q, Quaternion R)
{
	w = Q.w*R.w - Q.x*R.x - Q.y*R.y - Q.z*R.z;
	x = Q.w*R.x + Q.x*R.w + Q.y*R.z - Q.z*R.y;
	y = Q.w*R.y + Q.y*R.w + Q.z*R.x - Q.x*R.z;
	z = Q.w*R.z + Q.z*R.w + Q.x*R.y - Q.y*R.x;
}

void Quaternion::Rotate(Scalar radian, Scalar X, Scalar Y, Scalar Z)
{
	Scalar s;
	s = sin(radian/2);
	w = cos(radian/2);
	x = X*s;
	y = Y*s;
	z = Z*s;
}

Scalar Quaternion::Normalize()
{
	Scalar m = sqrt(w*w + x*x + y*y + z*z);
	
	w /= m;
	x /= m;
	y /= m;
	z /= m;
	
	return m;
}

void Quaternion::Convert(Scalar matrix [])
{
	Scalar xx, yy, zz, xy, wz, xz, wy, yz, wx;
	
	Normalize();
	
	xx = x*x;
	yy = y*y;
	zz = z*z;
	xy = x*y;
	wz = w*z;
	xz = x*z;
	wy = w*y;
	yz = y*z;
	wx = w*x;
	
	matrix[  0 ] = 1.0 - 2.0 * (yy + zz);
	matrix[  1 ] =       2.0 * (xy + wz);
	matrix[  2 ] =       2.0 * (xz - wy);
	matrix[  3 ] = 0.0;
	matrix[  4 ] =       2.0 * (xy - wz);
	matrix[  5 ] = 1.0 - 2.0 * (xx + zz);
	matrix[  6 ] =       2.0 * (yz + wx);
	matrix[  7 ] = 0.0;
	matrix[  8 ] =       2.0 * (xz + wy);
	matrix[  9 ] =       2.0 * (yz - wx);
	matrix[ 10 ] = 1.0 - 2.0 * (xx + yy);
	matrix[ 11 ] = 0.0;
	matrix[ 12 ] = 0.0;
	matrix[ 13 ] = 0.0;
	matrix[ 14 ] = 0.0;
	matrix[ 15 ] = 1.0;
}

Vector Quaternion::Direction()
{
	Vector V;
	
	V.x =       2.0 * (x*z - w*y);
	V.y =       2.0 * (y*z + w*x);
	V.z = 1.0 - 2.0 * (x*x + y*y);
	
	V.Normalize();
	return V;
}

