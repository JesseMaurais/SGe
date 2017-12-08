#include "Matrix.hpp"
#include <cmath>

Matrix::Matrix()
{
	for (int i = 0; i < 4; ++i)
	{
		 for (int j = 0; j < 4; ++j)
		 {
			 dim[i][j] = (i == j);
		 }
	}
}

void Matrix::Concatenate(Matrix const &M, Matrix const &N)
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
	n.W.x = x;
	n.W.y = y;
	n.W.z = z;
}

void Matrix::Scale(Scalar x, Scalar y, Scalar z)
{
	n.X.x = x;
	n.Y.y = y;
	n.Z.z = z;
}

void Matrix::Rotate(Scalar radian, Scalar x, Scalar y, Scalar z)
{
	Scalar xx, yy, zz,  xy, yz, zx,  sx, sy, sz,  s, c, a, m;

	s = std::sin(radian);
	c = std::cos(radian);
	a = 1 - c;
	m = std::sqrt(x*x + y*y + z*z);
	
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
	
	n.X.x = a * xx + c;
	n.Y.x = a * xy + sz;
	n.Z.x = a * zx - sy;
	
	n.X.y = a * xy - sz;
	n.Y.y = a * yy + c;
	n.Z.y = a * yz + sx;
	
	n.X.z = a * zx + sy;
	n.Y.z = a * yz - sx;
	n.Z.z = a * zz + c;
}

Vector Matrix::Direction() const
{
	Vector U;

	U.x = n.X.z;
	U.y = n.Y.z;
	U.z = n.Z.z;

	U.Normalize();
	return U;
}

Vector Matrix::Position() const
{
	Vector U;

	U.x = n.W.x;
	U.y = n.W.y;
	U.z = n.W.z;

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

Vector Matrix::Transform(Vector const &V)
{
	Vector U;
	
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			U[i] += V[j] * dim[j][i];
		}
	}
	
	return U;
}

void Matrix::Tangent(Vector const V [3], Vector const C [3])
{
	// Distances

	Vector V1 = V[0] - V[1];
	Vector V2 = V[0] - V[2];
	
	Vector C1 = C[0] - C[1];
	Vector C2 = C[0] - C[2];

	// Denominator

	Scalar den = C1.x * C2.y - C2.x * C1.y;

	if (den == 0) return;
	
	Vector T, B, N;

	// Tangent
	T.x = (C2.y * V1.x - C1.y * V2.x) / den;
	T.y = (C2.y * V1.y - C1.y * V2.y) / den;
	T.z = (C2.y * V1.z - C1.y * V2.z) / den;
	// Binormal
	B.x = (C1.x * V2.x - C2.x * V1.x) / den;
	B.y = (C1.x * V2.y - C2.x * V1.y) / den;
	B.z = (C1.x * V2.z - C2.x * V1.z) / den;
	// Normal
	N.Cross(T,B);
	
	// Determinant 
	den = (T.x * B.y * N.z - T.z * B.y * N.x)
	    + (B.x * N.y * T.z - B.z * N.y * T.x)
	    + (N.x * T.y * B.z - N.z * T.y * B.x);
	  
	Vector X,Y,Z;
	// Inverse
	X.Cross(B, N);
	Y.Cross(N, T);
	Z.Cross(T, B);
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
		dim[0][n] = T[n];
		dim[1][n] = B[n];
		dim[2][n] = N[n];
	}
}

Matrix::operator Scalar const *() const
{
	return dim[0];
}

Matrix::operator Scalar *()
{
	return dim[0];
}
