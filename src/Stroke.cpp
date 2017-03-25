#include "Stroke.hpp"

Stroke::Stroke(Scalar s)
{
	Steps(s);
}

void Stroke::Steps(Scalar s)
{
	step = s;
}

void Stroke::Curve(Scalar n)
{
	if (step) n /= step;
	
	for (Scalar i = 1; i < n; ++i)
	{
		Vector V;
		Scalar t = i/n;
		V = Bezier::Equate(t);
		Tessel::PolygonVertex(V);
	}
}

void Stroke::Move(Vector to)
{
	if (!moving)
	{
		moving = true;
		Tessel::Contour();
	}

	from = to;
}

void Stroke::Line(Vector to)
{
	if (moving)
	{
		moving = false;
		Tessel::PolygonVertex(from);
	}

	Tessel::PolygonVertex(to);
	from = to;
}

void Stroke::Cone(Vector X, Vector to)
{
	if (moving)
	{
		moving = false;
		Tessel::PolygonVertex(from);
	}
	
	Scalar n = (to - from).Magnitude();
	Bezier::Cone(from, X, to);
	Curve(n);
	from = to;
}

void Stroke::Cube(Vector X, Vector Y, Vector to)
{
	if (moving)
	{
		moving = false;
		Tessel::PolygonVertex(from);
	}
	
	Scalar n = (to - from).Magnitude();
	Bezier::Cube(from, X, Y, to);
	Curve(n);
	from = to;
}

