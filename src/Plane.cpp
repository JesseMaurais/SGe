#include "Plane.hpp"

Plane::Plane()
{
	distance = 0;
}

Plane::Plane(Vector const &N, Vector const &V)
{
	normal = N;
	distance = Dot(V);
}

Scalar Plane::Dot(Vector const &V) const
{
	return normal.Dot(V);
}

Scalar Plane::Equate(Vector const &V) const
{
	return Dot(V) - distance;
}

bool Plane::Agree(Vector const &N) const
{
	return Dot(N) > 0;
}

Scalar Plane::Cut(Vector const &U, Vector const &V) const
{
	return - Equate(U) / Dot(V - U);
}

Plane::Crossing Plane::Classify(Vector const *V, int n) const
{
	int planar=0, front=0, back=0;
	
	for (int i = 0; i < n; ++i)
	{
	 Scalar dot = Equate(V[i]);
	
	 if (dot < 0) ++back;
	  else
	 if (dot > 0) ++front;
	  else
	   ++planar;

	 if (front && back)
	  return Spanning;
	}
	
	if (planar == n) return Planar;
	 else
	if (front == 0) return Back;
	 else
	if (back == 0) return Front;
	 else
	  return Spanning;
}

