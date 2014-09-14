#include "Starship.hpp"
#include "Hedron.hpp"
#include "Model.hpp"
#include <cassert>
#include <cstdio>

void Model::Cube(double size)
{
	double l = 0.5*size;

	Begin(GL_QUADS);

	Vertex(+l, +l, -l);
	Vertex(-l, +l, -l);
	Vertex(-l, +l, +l);
	Vertex(+l, +l, +l);

	Vertex(+l, -l, +l);
	Vertex(-l, -l, +l);
	Vertex(-l, -l, -l);
	Vertex(+l, -l, -l);
 
	Vertex(+l, +l, +l);
	Vertex(-l, +l, +l);
	Vertex(-l, -l, +l);
	Vertex(+l, -l, +l);
 
	Vertex(+l, -l, -l);
	Vertex(-l, -l, -l);
	Vertex(-l, +l, -l);
	Vertex(+l, +l, -l);
 
	Vertex(-l, +l, +l);
	Vertex(-l, +l, -l);
	Vertex(-l, -l, -l);
	Vertex(-l, -l, +l);
 
	Vertex(+l, +l, -l);
	Vertex(+l, +l, +l);
	Vertex(+l, -l, +l);
	Vertex(+l, -l, -l);

	End();
}

