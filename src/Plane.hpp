#ifndef Plane_hpp
#define Plane_hpp

#include "Number.hpp"
#include "Vector.hpp"

struct Plane
{
	enum Crossing { Planar, Spanning, Front, Back };

	Vector normal;
	Scalar distance;

	Plane();
	Plane(Vector const &N, Vector const &V);
	Scalar Dot(Vector const &V) const;
	Scalar Equate(Vector const &V) const;
	Scalar Cut(Vector const &U, Vector const &V) const;
	bool Agree(Vector const &N) const;
	Crossing Classify(Vector const *V, int n=3) const;
};

#endif // file
