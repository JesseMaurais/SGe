#include "Starship.hpp"
#include "Model.hpp"

void Model::Cube(Scalar size)
{
	Scalar l = 0.5*size;

	Begin(Quads);

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

