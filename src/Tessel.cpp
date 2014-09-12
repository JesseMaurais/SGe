#include "Tessel.hpp"
#include "SDL.hpp"
#include <cstdio>
#include <stdexcept>

Tessel::Tessel()
{
	obj = gluNewTess();
	if (!obj) throw std::bad_alloc();

	typedef _GLUfuncptr FP;
	
	gluTessCallback(obj, GLU_TESS_COMBINE_DATA, FP(combine));
	gluTessCallback(obj, GLU_TESS_VERTEX_DATA, FP(next));
	gluTessCallback(obj, GLU_TESS_BEGIN_DATA, FP(begin));
	gluTessCallback(obj, GLU_TESS_END_DATA, FP(end));
	gluTessCallback(obj, GLU_TESS_ERROR, FP(error));
}

Tessel::~Tessel()
{
	gluDeleteTess(obj);
}

void Tessel::Vertex(int index, double *v)
{
	gluTessVertex(obj, v, (void*) index);
}

void Tessel::Vertex(Vector vertex)
{
	int index = Mesh::AddVertex(vertex);
	Vector &V = Mesh::GetVertex(index);
	Vertex(index, V.v);
}

void Tessel::BeginPolygon()
{
	gluTessBeginPolygon(obj, this);
	gluTessBeginContour(obj);
}

void Tessel::Contour()
{
	gluTessEndContour(obj);
	gluTessBeginContour(obj);
}

void Tessel::EndPolygon()
{
	gluTessEndContour(obj);
	gluTessEndPolygon(obj);
}

void Tessel::combine(double *v, void*, void*, int *index, MeshComposer *self)
{
	Vector V(v[0], v[1], v[2]);
	*index = self->AddVertex(V);
}

void Tessel::error(int code)
{
	SDL_Log("%s: %s\n", __func__, gluErrorString(code));
}

void Tessel::begin(int mode, MeshComposer *self)
{
	self->Begin(mode);
}

void Tessel::next(int index, MeshComposer *self)
{
	self->Next(index);
}

void Tessel::end(MeshComposer *self)
{
	self->End();
}

