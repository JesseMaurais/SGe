#include "Crayon.hpp"
#include "Bezier.hpp"
#include "OpenGL.hpp"
#include <cmath>

Crayon::Crayon(void)
{
	JoinStyle(JoinRound);
	CapStyle(CapButt);
	Width(4);
	step = -1;
}

void Crayon::JoinStyle(enum Join style)
{
	join = style;
}

void Crayon::CapStyle(enum Cap style)
{
	cap = style;
}

void Crayon::Width(double thickness)
{
	width = thickness;
}

void Crayon::Begin(void)
{
	step = 0;
}

void Crayon::End(void)
{
	if (step >= 0)
	{
	 step = -1;
	 DrawCap();
	}
}

void Crayon::Vertex(Vector point)
{
	if (step < 0) return;

	++step;

	if (step > 1)
	{
	 lineB = point - pivot;
	 lineB.Normalize();
	 perpB = Perpendicular(lineB);

	 DrawSegment(point);

	 if (step > 2)
	  DrawJoin();
	 else
	  DrawCap();

	 lineA = lineB;
	 perpA = perpB;
	}

	pivot = point;
}

Vector Crayon::Perpendicular(Vector N)
{
	const Vector Z(0, 0, 1);
	N.Cross(N, Z);
	return N;
}

// Line

void Crayon::DrawSegment(Vector point)
{
	Vector A = pivot + perpB * width;
	Vector B = pivot - perpB * width;
	Vector C = point - perpB * width;
	Vector D = point + perpB * width;

	glBegin(GL_LINE_LOOP);
	 glVertex3dv(A.v);
	 glVertex3dv(B.v);
	 glVertex3dv(C.v);
	 glVertex3dv(D.v);
	glEnd();
}

// Join

void Crayon::DrawJoin(void)
{
	double d = perpA.Dot(lineB);

	Vector U,V;

	if (d < 0)
	{
	 U = pivot + perpA * width;
	 V = pivot + perpB * width;
	}
	else
	if (d > 0)
	{
	 U = pivot - perpA * width;
	 V = pivot - perpB * width;
	}
	else
	 return;

	switch (join)
	{
	 case JoinBevel:
	  DrawJoinBevel(U, V);
	  break;
	 case JoinMiter:
	  DrawJoinMiter(U, V, d);
	  break;
	 case JoinRound:
	  DrawJoinRound(U, V);
	  break;
	 case JoinConic:
	  DrawJoinConic(U, V, d);
	  break;
	}
}

void Crayon::DrawJoinBevel(Vector U, Vector V)
{
	glBegin(GL_LINE_LOOP);
	 glVertex3dv(pivot.v);
	 glVertex3dv(U.v);
	 glVertex3dv(V.v);
	glEnd();
}

void Crayon::DrawJoinMiter(Vector U, Vector V, double d)
{
	Vector W = pivot + (lineA - lineB) * width / fabs(d);

	glBegin(GL_LINE_LOOP);
	 glVertex3dv(pivot.v);
	 glVertex3dv(U.v);
	 glVertex3dv(W.v);
	 glVertex3dv(V.v);
	glEnd();
}

void Crayon::DrawJoinRoundPart(Vector U, Vector V, int n)
{
	Vector UV = V - U;

	for (float m = 1; m < n; ++m)
	{
		Vector W = U + UV * m/n;

		W = W - pivot;
		W.Normalize();
		W = W * width;
		W = W + pivot;
		glVertex3dv(W.v);
	}
}

void Crayon::DrawJoinRound(Vector U, Vector V)
{
	double d = lineA.Dot(lineB);

	glBegin(GL_LINE_LOOP);
	 glVertex3dv(pivot.v);
	 glVertex3dv(U.v);

	 if (d < 0)
	 {
		Vector W = lineA * width + pivot;

		DrawJoinRoundPart(U, W, 5);
		DrawJoinRoundPart(W, V, 5);
	 }
	 else
		DrawJoinRoundPart(U, V, 9);

	 glVertex3dv(V.v);
	glEnd();
}

void Crayon::DrawJoinConic(Vector U, Vector V, double d)
{
	Vector W = pivot + (lineA - lineB) * width / fabs(d);

	Bezier bez;

	glBegin(GL_LINE_LOOP);
	 glVertex3dv(pivot.v);
	 glVertex3dv(U.v);

	 bez.Cone(U, W, V);

	 for (float n = 1; n < 100; ++n)
	 {
	  glVertex3dv(bez.Equate(n/100).v);
	 }

	 glVertex3dv(V.v);
	glEnd();
}

// Cap

void Crayon::DrawCap()
{
	switch (cap)
	{
	 case CapSquare:
	  DrawCapSquare();
	  break;
	 case CapRound:
	  DrawCapRound();
	  break;
	 case CapButt:
	  // By fault
	  break;
	}
}

void Crayon::DrawCapSquare()
{
	//TODO
}

void Crayon::DrawCapRound()
{
	//TODO
}

