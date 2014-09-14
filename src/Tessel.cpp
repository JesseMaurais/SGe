#include "Tessel.hpp"
#include "SDL.hpp"
#include <cstdio>
#include <stdexcept>

Tessel::Tessel()
{
	obj = gluNewTess();
	if (!obj) throw std::bad_alloc();

	typedef _GLUfuncptr FP;
	
	gluTessCallback(obj, GLU_TESS_COMBINE_DATA, (FP) combine);
	gluTessCallback(obj, GLU_TESS_VERTEX_DATA, (FP) next);
	gluTessCallback(obj, GLU_TESS_BEGIN_DATA, (FP) begin);
	gluTessCallback(obj, GLU_TESS_END_DATA, (FP) end);
	gluTessCallback(obj, GLU_TESS_ERROR, (FP) error);
}

Tessel::~Tessel()
{
	gluDeleteTess(obj);
}

void Tessel::Property(int name, double param)
{
	gluTessProperty(obj, name, param);
}

void Tessel::BeginPolygon()
{
	gluTessBeginPolygon(obj, this);
	gluTessBeginContour(obj);
}

void Tessel::PolygonVertex(int point)
{
	Vector &V = Mesh::GetVertex(point);
	gluTessVertex(obj, V.v, point);
}

void Tessel::PolygonVertex(Vector V)
{
	int index = Mesh::AddVertex(V);
	PolyVertex(index);
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

void Tessel::error(int code)
{
	SDL_Log("%s: %s\n", __func__, gluErrorString(code));
}

void Tessel::combine(double *v, void*, void*, int *index, MeshComposer *self)
{
	*index = self->Vertex(v[0], v[1], v[2]);
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

