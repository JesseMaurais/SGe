#include "Vector.hpp"
#include <cmath>

Vector::Vector(void)
{
	x = y = z = 0;
}

Vector::Vector(double *p)
{
	x = p[0]; y = p[1]; z = p[2];
}

Vector::Vector(double a)
{
	x = y = z = a;
}

Vector::Vector(double a, double b)
{
	x = a; y = b; z = 0;
}

Vector::Vector(double a, double b, double c)
{
	x = a; y = b; z = c;
}

double Vector::Quadratic(double t)
{
	return x*t*t + y*t + z;
}

double Vector::Linear(double t)
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

Vector Vector::operator + (double t)
{
	return Vector(x+t, y+t, z+t);
}

Vector Vector::operator - (double t)
{
	return Vector(x-t, y-t, z-t);
}

Vector Vector::operator * (double s)
{
	return Vector(x*s, y*s, z*s);
}

Vector Vector::operator / (double s)
{
	return Vector(x/s, y/s, z/s);
}

double Vector::Dot(Vector V)
{
	return x*V.x + y*V.y + z*V.z;
}

void Vector::Cross(Vector U, Vector V)
{
	x = U.y*V.z - U.z*V.y;
	y = U.z*V.x - U.x*V.z;
	z = U.x*V.y - U.y*V.x;
}

double Vector::Square(void)
{
	return Dot(*this);
}

double Vector::Magnitude(void)
{
	return sqrt(Square());
}

double Vector::Normalize(void)
{
	double m = Magnitude();

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

