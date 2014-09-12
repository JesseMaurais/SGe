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
	 Vertex(Equate(i/n));
	}
}

void Stroke::Move(Vector to)
{
	if (!moving)
	{
	 moving = true;
	 Contour();
	}
	from = to;
}

void Stroke::Line(Vector to)
{
	if (moving)
	{
	 moving = false;
	 Vertex(from);
	}
	Vertex(to);
	from = to;
}

void Stroke::Cone(Vector X, Vector to)
{
	if (moving)
	{
	 moving = false;
	 Vertex(from);
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
	 Vertex(from);
	}
	
	double n = (to - from).Magnitude();
	
	Bezier::Cube(from, X, Y, to);
	Curve(n);
	from = to;
}

