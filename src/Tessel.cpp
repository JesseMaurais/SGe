#include "Tessel.hpp"
#include "OpenGL.hpp"
#include "SDL.hpp"
#include <stdexcept>

static void Error(int code)
{
	SDL_Log("Tessel: %s", gluErrorString(code));
}

static void Combine(double *v, void *data[4], float *weight, int *index, MeshComposer *self)
{
	(void) data;
	(void) weight;
	*index = self->Vertex(v[0], v[1], v[2]);
}

static void Begin(int mode, MeshComposer *self)
{
	switch (mode)
	{
	case GL_TRIANGLE_FAN:
		self->Begin(MeshComposer::Fan);
		break;
	case GL_TRIANGLE_STRIP:
		self->Begin(MeshComposer::Strip);
		break;
	case GL_TRIANGLES:
		self->Begin(MeshComposer::Triangles);
		break;
	}
}

static void Next(int index, MeshComposer *self)
{
	self->Next(index);
}

static void End(MeshComposer *self)
{
	self->End();
}

Tessel::Tessel()
{
	obj = gluNewTess();
	if (not obj) throw std::bad_alloc();
	gluTessCallback(obj, GLU_TESS_ERROR, (_GLUfuncptr)::Error);
	gluTessCallback(obj, GLU_TESS_COMBINE_DATA, (_GLUfuncptr)::Combine);
	gluTessCallback(obj, GLU_TESS_VERTEX_DATA, (_GLUfuncptr)::Next);
	gluTessCallback(obj, GLU_TESS_BEGIN_DATA, (_GLUfuncptr)::Begin);
	gluTessCallback(obj, GLU_TESS_END_DATA, (_GLUfuncptr)::End);
}

Tessel::~Tessel()
{
	gluDeleteTess(obj);
}

void Tessel::BeginPolygon()
{
	gluTessBeginPolygon(obj, this);
	gluTessBeginContour(obj);
}

void Tessel::PolygonVertex(int point)
{
	Vector &V = Mesh::GetVertex(point);
	gluTessVertex(obj, V, reinterpret_cast<void*>(point));
}

void Tessel::PolygonVertex(Vector V)
{
	int index = Mesh::AddVertex(V);
	PolygonVertex(index);
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
