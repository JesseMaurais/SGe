#include "Starship.hpp"
#include "Hedron.hpp"
#include "Model.hpp"

void Model::Cube(double size)
{
	double l = 0.5*size;

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

