#include "Vector.hpp"
#include <cmath>

Vector::Vector()
{
	x = y = z = Scalar(0);
}

Vector::Vector(Scalar *v)
{
	x = v[0]; y = v[1]; z = v[2];
}

Vector::Vector(Scalar a)
{
	x = y = z = a;
}

Vector::Vector(Scalar a, Scalar b)
{
	x = a; y = b; z = 0;
}

Vector::Vector(Scalar a, Scalar b, Scalar c)
{
	x = a; y = b; z = c;
}

Scalar Vector::Dot(Vector const &V) const
{
	return x*V.x + y*V.y + z*V.z;
}

void Vector::Cross(Vector const &U, Vector const &V)
{
	x = U.y*V.z - U.z*V.y;
	y = U.z*V.x - U.x*V.z;
	z = U.x*V.y - U.y*V.x;
}

Scalar Vector::Square() const
{
	return Dot(*this);
}

Scalar Vector::Magnitude() const
{
	return std::sqrt(Square());
}

Scalar Vector::Normalize()
{
	Scalar m = Magnitude();

	x /= m;
	y /= m;
	z /= m;

	return m;
}

Scalar Vector::Right(Vector const &A, Vector const &B, Vector const &C)
{
	Cross(A - B, A - C);
	return Normalize();
}

Vector Vector::Project(Vector const &V) const
{
	Vector U = V;
	U.Normalize();
	return V*Dot(U);
}

Scalar Vector::Quadratic(Scalar t) const
{
	return std::fma(Linear(t), t, z);
}

Scalar Vector::Linear(Scalar t) const
{
	return std::fma(x, t, y);
}

Vector Vector::operator - () const
{
	return Vector(-x, -y, -z);
}

Vector Vector::operator + (Vector const &V) const
{
	return Vector(x+V.x, y+V.y, z+V.z);
}

Vector Vector::operator - (Vector const &V) const
{
	return Vector(x-V.x, y-V.y, z-V.z);
}

Vector Vector::operator * (Vector const &V) const
{
	return Vector(x*V.x, y*V.y, z*V.z);
}

Vector Vector::operator / (Vector const &V) const
{
	return Vector(x/V.x, y/V.y, z/V.z);
}

Vector Vector::operator + (Scalar t) const
{
	return Vector(x+t, y+t, z+t);
}

Vector Vector::operator - (Scalar t) const
{
	return Vector(x-t, y-t, z-t);
}

Vector Vector::operator * (Scalar s) const
{
	return Vector(x*s, y*s, z*s);
}

Vector Vector::operator / (Scalar s) const
{
	return Vector(x/s, y/s, z/s);
}

bool Vector::operator == (Vector const &V) const
{
	return (x == V.x) and (y == V.y) and (z == V.z);
}

