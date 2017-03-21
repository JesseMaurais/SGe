#include "Nurbs.hpp"
#include "OpenGL.hpp"
#include "SDL.hpp"
#include <stdexcept>

static void Error(int code)
{
	SDL_Log("Nurbs: %s", gluErrorString(code));
}

static void Begin(int mode, MeshComposer *self)
{
	switch (mode)
	{
	case GL_TRIANGLE_FAN:
		self->Begin(MeshComposer::Fan);
		break;
	case GL_QUAD_STRIP:
	case GL_TRIANGLE_STRIP:
		self->Begin(MeshComposer::Strip);
		break;
	case GL_TRIANGLES:
		self->Begin(MeshComposer::Triangles);
		break;
	}
}

static void End(MeshComposer *self)
{
	self->End();
}

static void Vertex(float *v, MeshComposer *self)
{
	int index = self->Vertex(v[0], v[1], v[2]);
	self->Next(index);
}

static void TexCoord(float *v, MeshComposer *self)
{
	self->TexCoord(v[0], v[1], v[2]);
}

static void Normal(float *v, MeshComposer *self)
{
	self->Normal(v[0], v[1], v[2]);
}

static void Color(float *v, MeshComposer *self)
{
	self->Color(v[0], v[1], v[2]);
}

Nurbs::Nurbs()
{
	obj = gluNewNurbsRenderer();
	if (not obj) throw std::bad_alloc();


	gluNurbsCallbackData(obj, this);
	gluNurbsCallback(obj, GLU_NURBS_ERROR, (_GLUfuncptr)::Error);
	gluNurbsCallback(obj, GLU_NURBS_BEGIN_DATA, (_GLUfuncptr)::Begin);
	gluNurbsCallback(obj, GLU_NURBS_END_DATA, (_GLUfuncptr)::End);
	gluNurbsCallback(obj, GLU_NURBS_VERTEX_DATA, (_GLUfuncptr)::Vertex);
	gluNurbsCallback(obj, GLU_NURBS_NORMAL_DATA, (_GLUfuncptr)::Normal);
	gluNurbsCallback(obj, GLU_NURBS_TEXTURE_COORD_DATA, (_GLUfuncptr)::TexCoord);
	gluNurbsCallback(obj, GLU_NURBS_COLOR_DATA, (_GLUfuncptr)::Color);
}

Nurbs::~Nurbs()
{
	gluDeleteNurbsRenderer(obj);
}

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


