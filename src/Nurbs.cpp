#include "SDL.hpp"
#include "Nurbs.hpp"
#include <stdexcept>

Nurbs::Nurbs()
{
	obj = gluNewNurbsRenderer();
	if (!obj) throw std::bad_alloc();

	typedef _GLUfuncptr FP;

	gluNurbsCallbackData(obj, this);
	gluNurbsCallback(obj, GLU_NURBS_ERROR, FP(error));
	gluNurbsCallback(obj, GLU_NURBS_BEGIN_DATA, FP(begin));
	gluNurbsCallback(obj, GLU_NURBS_END_DATA, FP(end));
	gluNurbsCallback(obj, GLU_NURBS_VERTEX_DATA, FP(vertex));
	gluNurbsCallback(obj, GLU_NURBS_NORMAL_DATA, FP(normal));
	gluNurbsCallback(obj, GLU_NURBS_TEXTURE_COORD_DATA, FP(texCoord));
}

Nurbs::~Nurbs()
{
	gluDeleteNurbsRenderer(obj);
}

void Nurbs::error(int code)
{
	SDL_Log("%s: %s\n", __func__, gluErrorString(code));
}

void Nurbs::begin(int mode, MeshComposer *self)
{
	self->Begin(mode);
}

void Nurbs::end(MeshComposer *self)
{
	self->End();
}

void Nurbs::vertex(float *v, MeshComposer *self)
{
	Vector V(v[0], v[1], v[2]);
	int index = self->AddVertex(V);
	pivot.vertex = index;
	
	index = self->AddPoint(pivot);
	self->Next(index);
}

void Nurbs::texCoord(float *v, MeshComposer *self)
{
	Vector V(v[0], v[1], v[2]);
	int index = self->AddTexCoord(V);
	pivot.texCoord = index;
}

void Nurbs::normal(float *v, MeshComposer *self)
{
	Vector V(v[0], v[1], v[2]);
	int index = self->AddNormal(V);
	pivot.normal = index;
}

Point Nurbs::pivot;

void Nurbs::BeginCurve()
{
	gluBeginCurve(obj);
}

void Nurbs::EndCurve()
{
	gluEndCurve(obj);
}

void Nurbs::NurbsCurve(int n, float *knots, int stride, float *points, int order, int type)
{
	gluNurbsCurve(obj, n, knots, stride, points, order, type);
}

void Nurbs::PwlCurve(int n, float *data, int stride, int type)
{
	gluPwlCurve(obj, n, data, stride, type);
}

void Nurbs::BeginTrim()
{
	gluBeginTrim(obj);
}

void Nurbs::EndTrim()
{
	gluEndTrim(obj);
}

void Nurbs::BeginSurface()
{
	gluBeginSurface(obj);
}

void Nurbs::NurbsSurface(int n, float *nknots, int nstride, int norder, int m, float *mknots, int mstride, int morder, float *points, int type)
{
	gluNurbsSurface(obj, n, nknots, m, mknots, nstride, mstride, points, norder, morder, type);
}

void Nurbs::EndSurface()
{
	gluEndSurface(obj);
}


