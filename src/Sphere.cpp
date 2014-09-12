#include "Sphere.hpp"
#include "OpenGL.hpp"
#include "Metric.hpp"
#include <cmath>

void Sphere::Vertex(Vector &V, double s, double t)
{
	glNormal3dv(V.v);
	glTexCoord2d(s,t);
	glVertex3dv(V.v);
}

void Sphere::Draw(Vector V [3])
{
	for (register int i=0, j=1, k=2; i<3; j=k, k=i, ++i)
	{
		// Close seam at poles

		Vector U = V[i];

		if (U.x == 0 && U.z == 0)
		{
		 U.x = V[j].x + V[k].x;
		 U.z = V[j].z + V[k].z;
		}

		// Calculate texture coordinates

		double s = atan2(U.x,U.z) / (Pi*2);
		double t = 1.0 - acos(U.y) / Pi;

		// Close seam at 0/2pi overlap

		if (U.z <= 0 && V[j].z <= 0 && V[k].z <= 0)
		{
		 if (U.x < 0 && (V[j].x >= 0 || V[k].x >= 0))
		 {
		  s += 1.0;
		 }
		}

		// New point

		Vertex(V[i], s, t);
	}
}

void Sphere::Divide(unsigned depth, Vector V [3])
{
	if (!depth)
	{
	 Draw(V);
	 return;
	}
	else --depth;

	Vector U [3];

	for (int i=0, j=1, k=2; i<3; j=k, k=i, ++i)
	{
	 U[k] = V[i] + V[j];
	 U[k].Normalize();
	}

	Divide(depth, U);

	for (int i=0, j=1, k=2; i<3; j=k, k=i, ++i)
	{
	 Vector W [3] = { V[i], U[k], U[j] };

	 Divide(depth, W);
	}
}

void Sphere::Draw(unsigned depth)
{
	using namespace Icosahedron;

	glBegin(GL_TRIANGLES);

	for (int i = 0; i < nFaces; ++i)
	{
	 Vector V [3];

	 for (int j = 0; j < 3; ++j)
	 {
	  V[j] = Rects[Faces[i][j].box][Faces[i][j].point];
	  V[j].Normalize();
	 }

	 Divide(depth, V);
	}
	
	glEnd();
}

