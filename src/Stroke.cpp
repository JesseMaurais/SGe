#include "Stroke.hpp"

Stroke::Stroke(double size)
{
	Point(size);
}

void Stroke::Point(double size)
{
	step = size;
}

void Stroke::Curve(double n)
{
	if (step) n /= step;
	
	for (double i = 1; i < n; ++i)
	{
	 Vector V;
	 double t = i/n;
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
	
	double n = (to - from).Magnitude();
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
	
	double n = (to - from).Magnitude();
	
	Bezier::Cube(from, X, Y, to);
	Curve(n);
	from = to;
}

