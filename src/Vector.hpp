#ifndef Vector_hpp
#define Vector_hpp

#include "Number.hpp"

struct Vector
{
	Scalar x, y, z;

	Vector();
	Vector(Scalar *);
	Vector(Scalar a);
	Vector(Scalar a, Scalar b);
	Vector(Scalar a, Scalar b, Scalar c);

	Scalar Dot(Vector const &V) const;
	void Cross(Vector const &U, Vector const &V);
	Scalar Square() const;
	Scalar Magnitude() const;
	Scalar Normalize();
	Scalar Right(Vector const &A, Vector const &B, Vector const &C);
	Vector Project(Vector const &V) const;
	Scalar Quadratic(Scalar t) const;
	Scalar Linear(Scalar t) const;

	Vector operator - () const;
	Vector operator + (Vector const &V) const;
	Vector operator - (Vector const &V) const;
	Vector operator * (Vector const &V) const;
	Vector operator / (Vector const &V) const;
	Vector operator + (Scalar t) const;
	Vector operator - (Scalar t) const;
	Vector operator * (Scalar s) const;
	Vector operator / (Scalar s) const;
	bool operator == (Vector const &V) const;
	Scalar const &operator[](int index) const;
	Scalar &operator[](int index);
	operator Scalar const *() const;
	operator Scalar *();
};

#endif // file

