#ifndef __Nurbs__
#define __Nurbs__

#include "Mesh.hpp"
#include "OpenGL.hpp"

struct Nurbs : virtual MeshComposer
{	
	Nurbs();
	~Nurbs();

	void BeginCurve();
	void NurbsCurve(int n, float *knots, int stride, float *points, int order, int type);
	void PwlCurve(int n, float *data, int stride, int type);
	void EndCurve();

	void BeginTrim();
	void EndTrim();

	void BeginSurface();
	void NurbsSurface(int n, float *nknots, int nstride, int norder, int m, float *mknots, int mstride, int morder, float *points, int type);
	void EndSurface();

 protected:

	GLUnurbs *obj;
	
 private:
 
	static void error(int code);
	static void begin(int mode, MeshComposer *self);
	static void end(MeshComposer *self);
	static void vertex(float *v, MeshComposer *self);
	static void texCoord(float *v, MeshComposer *self);
	static void normal(float *v, MeshComposer *self);
	static void color(float *v, MeshComposer *self);
};

#endif // file

