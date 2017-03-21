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

	Scalar Dot(Vector V);
	void Cross(Vector U, Vector V);
	Scalar Square();
	Scalar Magnitude();
	Scalar Normalize();
	void Right(Vector A, Vector B, Vector C);
	Vector Project(Vector V);
	Scalar Quadratic(Scalar t);
	Scalar Linear(Scalar t);

	Vector operator - ();
	Vector operator + (Vector V);
	Vector operator - (Vector V);
	Vector operator * (Vector V);
	Vector operator / (Vector V);
	Vector operator + (Scalar t);
	Vector operator - (Scalar t);
	Vector operator * (Scalar s);
	Vector operator / (Scalar s);
	bool operator == (Vector V);
};

#endif // file

