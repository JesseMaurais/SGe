#ifndef Nurbs_hpp
#define Nurbs_hpp

#include "Mesh.hpp"

class GLUnurbs;

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
};

#endif // file

