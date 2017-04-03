#ifndef Vector_hpp
#define Vector_hpp

#include "Number.hpp"

struct Vector
{
	union
	{
	 Scalar v[3];
	
	 struct { Scalar x,y,z; };
	 struct { Scalar i,j,k; };
	 struct { Scalar s,t,r; };
	 struct { Scalar R,G,B; };
	};

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
};

#endif // file

