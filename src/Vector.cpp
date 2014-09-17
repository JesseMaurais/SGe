#include "Vector.hpp"
#include <cmath>

Vector::Vector()
{
	x = y = z = 0;
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

Scalar Vector::Quadratic(Scalar t)
{
	return x*t*t + y*t + z;
}

Scalar Vector::Linear(Scalar t)
{
	return x*t + y;
}

Vector Vector::operator - (void)
{
	return Vector(-x, -y, -z);
}

Vector Vector::operator + (Vector V)
{
	return Vector(x+V.x, y+V.y, z+V.z);
}

Vector Vector::operator - (Vector V)
{
	return Vector(x-V.x, y-V.y, z-V.z);
}

Vector Vector::operator * (Vector V)
{
	return Vector(x*V.x, y*V.y, z*V.z);
}

Vector Vector::operator / (Vector V)
{
	return Vector(x/V.x, y/V.y, z/V.z);
}

Vector Vector::operator + (Scalar t)
{
	return Vector(x+t, y+t, z+t);
}

Vector Vector::operator - (Scalar t)
{
	return Vector(x-t, y-t, z-t);
}

Vector Vector::operator * (Scalar s)
{
	return Vector(x*s, y*s, z*s);
}

Vector Vector::operator / (Scalar s)
{
	return Vector(x/s, y/s, z/s);
}

Scalar Vector::Dot(Vector V)
{
	return x*V.x + y*V.y + z*V.z;
}

void Vector::Cross(Vector U, Vector V)
{
	x = U.y*V.z - U.z*V.y;
	y = U.z*V.x - U.x*V.z;
	z = U.x*V.y - U.y*V.x;
}

Scalar Vector::Square(void)
{
	return Dot(*this);
}

Scalar Vector::Magnitude(void)
{
	return sqrt(Square());
}

Scalar Vector::Normalize(void)
{
	Scalar m = Magnitude();

	x /= m;
	y /= m;
	z /= m;

	return m;
}

void Vector::Right(Vector A, Vector B, Vector C)
{
	Cross(A - B, A - C);
	Normalize();
}

Vector Vector::Project(Vector V)
{
	Vector U = V;
	U.Normalize();
	return V*Dot(U);
}

bool Vector::operator == (Vector V)
{
	return (x == V.x) && (y == V.y) && (z == V.z);
}

