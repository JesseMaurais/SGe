#include "Starship.hpp"
#include "Hedron.hpp"
#include "Model.hpp"

void Model::Cube(Scalar size)
{
	Scalar l = 0.5*size;

	Begin(GL_QUADS);

	Next(Vertex(+l, +l, -l));
	Next(Vertex(-l, +l, -l));
	Next(Vertex(-l, +l, +l));
	Next(Vertex(+l, +l, +l));

	Next(Vertex(+l, -l, +l));
	Next(Vertex(-l, -l, +l));
	Next(Vertex(-l, -l, -l));
	Next(Vertex(+l, -l, -l));
 
	Next(Vertex(+l, +l, +l));
	Next(Vertex(-l, +l, +l));
	Next(Vertex(-l, -l, +l));
	Next(Vertex(+l, -l, +l));
 
	Next(Vertex(+l, -l, -l));
	Next(Vertex(-l, -l, -l));
	Next(Vertex(-l, +l, -l));
	Next(Vertex(+l, +l, -l));
 
	Next(Vertex(-l, +l, +l));
	Next(Vertex(-l, +l, -l));
	Next(Vertex(-l, -l, -l));
	Next(Vertex(-l, -l, +l));
 
	Next(Vertex(+l, +l, -l));
	Next(Vertex(+l, +l, +l));
	Next(Vertex(+l, -l, +l));
	Next(Vertex(+l, -l, -l));

	End();
}

// NO! GO BREADTH FIRST
void Model::GeoSphere(Scalar radius, int depth)
{
	using namespace Icosahedron;
	int indexes[nFaces][4];

	Begin(GL_TRIANGLES);

	Vector V;

	for (int i = 0; i < nFaces; ++i)
	{
	 for (int j = 0; j < 3; ++j)
	 {
	  V = Rects[Faces[i][j].box][Faces[i][j].point];
	  V.Normalize();
	  V = V*radius;
	  indexes[i][j] = AddVertex(V);
	 }
	}

	Surface S;

	for (int i = 0; i < nFaces; ++i)
	{
	 for (int j = 0; j < 3; ++j)
	 {
	  S.points[j] = indexes[i][j];
	 }
	 Divide(depth, S);
	}
	
	End();
}

void Model::Divide(int depth, Surface S)
{
	if (!depth)
	{
	 AddSurface(S);
	 return;
	}
	else --depth;

	Vector V [3];
	GetVertexes(S.points, V);

	Surface R;
	for (int i=0, j=1, k=2; i<3; j=k, k=i, ++i)
	{
	 Vector U = (V[i] + V[j])/2;
	 R.points[k] = AddVertex(U);
	}

	Divide(depth, R);

	Surface Q;
	for (int i=0, j=1, k=2; i<3; j=k, k=i, ++i)
	{
	 Q.points[0] = S.points[i];
	 Q.points[1] = R.points[k];
	 Q.points[2] = S.points[j];
	 Divide(depth, Q);
	}
}

