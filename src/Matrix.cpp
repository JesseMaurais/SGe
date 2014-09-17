#include "Matrix.hpp"
#include <cmath>

Matrix::Matrix()
{
	for (int i = 0; i < 4; ++i)
	 for (int j = 0; j < 4; ++j)
	  dim[i][j] = (i == j);
}

void Matrix::Concatenate(Matrix M, Matrix N)
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			dim[i][j] = 0;
			for (int k = 0; k < 3; ++k)
			{
				dim[i][j] += N.dim[i][k] * M.dim[k][j];
			}
		}
	}
}

void Matrix::Translate(Scalar x, Scalar y, Scalar z)
{
	W.x = x;
	W.y = y;
	W.z = z;
}

void Matrix::Scale(Scalar x, Scalar y, Scalar z)
{
	X.x = x;
	Y.y = y;
	Z.z = z;
}

void Matrix::Rotate(Scalar radian, Scalar x, Scalar y, Scalar z)
{
	Scalar xx, yy, zz,  xy, yz, zx,  sx, sy, sz,  s, c, a, m;

	s = sin(radian);
	c = cos(radian);
	a = 1 - c;
	m = sqrt(x*x + y*y + z*z);
	
	x /= m;
	y /= m;
	z /= m;

	xx = x*x;
	yy = y*y;
	zz = z*z;
	xy = x*y;
	yz = y*z;
	zx = z*x;
	sx = x*s;
	sy = y*s;
	sz = z*s;
	
	X.x = a * xx + c;
	Y.x = a * xy + sz;
	Z.x = a * zx - sy;
	
	X.y = a * xy - sz;
	Y.y = a * yy + c;
	Z.y = a * yz + sx;
	
	X.z = a * zx + sy;
	Y.z = a * yz - sx;
	Z.z = a * zz + c;
}

Vector Matrix::Direction()
{
	Vector U;

	U.x = X.z;
	U.y = Y.z;
	U.z = Z.z;

	U.Normalize();
	return U;
}

Vector Matrix::Position()
{
	Vector U;

	U.x = W.x;
	U.y = W.y;
	U.z = W.z;

	return U;
}

void Matrix::Transpose()
{
	Scalar temporary;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = i + 1; j < 3; ++j)
		{
			temporary = dim[i][j];
			dim[i][j] = dim[j][i];
			dim[j][i] = temporary;
		}
	}
}

Vector Matrix::Transform(Vector V)
{
	Vector U;
	
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			U.v[i] += V.v[j] * dim[j][i];
		}
	}
	
	return U;
}

void Matrix::Tangent(Vector V [3], Vector C [3])
{
	// Distances

	Vector V1 = V[0] - V[1];
	Vector V2 = V[0] - V[2];
	
	Vector C1 = C[0] - C[1];
	Vector C2 = C[0] - C[2];

	// Denominator

	Scalar den = C1.s * C2.t - C2.s * C1.t;

	if (den == 0) return;
	
	Vector T,B,N;

	// Tangent
	T.x = (C2.t * V1.x - C1.t * V2.x) / den;
	T.y = (C2.t * V1.y - C1.t * V2.y) / den;
	T.z = (C2.t * V1.z - C1.t * V2.z) / den;
	// Binormal
	B.x = (C1.s * V2.x - C2.s * V1.x) / den;
	B.y = (C1.s * V2.y - C2.s * V1.y) / den;
	B.z = (C1.s * V2.z - C2.s * V1.z) / den;
	// Normal
	N.Cross(T,B);
	
	// Determinant 
	den = (T.x * B.y * N.z - T.z * B.y * N.x)
	    + (B.x * N.y * T.z - B.z * N.y * T.x)
	    + (N.x * T.y * B.z - N.z * T.y * B.x);
	  
	Vector X,Y,Z;
	// Inverse
	X.Cross(B,N);
	Y.Cross(N,T);
	Z.Cross(T,B);
	// Tangent
	T.x = +X.x/den;
	T.y = -Y.x/den;
	T.z = +Z.x/den;
	// Binormal
	B.x = -X.y/den;
	B.y = +Y.y/den;
	B.z = -Z.y/den;
	// Normal
	N.x = +X.z/den;
	N.y = -Y.z/den;
	N.z = +Z.z/den;
	// Unit size
	T.Normalize();
	B.Normalize();
	N.Normalize();

	// Store matrix

	for (int n = 0; n < 3; ++n)
	{
	 dim[0][n] = T.v[n];
	 dim[1][n] = B.v[n];
	 dim[2][n] = N.v[n];
	}
}

