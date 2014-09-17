#ifndef __Tessel__
#define __Tessel__

#include "OpenGL.hpp"
#include "Mesh.hpp"

struct Tessel : virtual MeshComposer
{
	Tessel();
	~Tessel();

	void BeginPolygon();
	void PolygonVertex(int point);
	void PolygonVertex(Vector);
	void EndPolygon();
	void Contour();

 protected:

	GLUtesselator *obj;

 private:

	static void combine(double *v, void*, void*, int *index, MeshComposer *self);
	static void begin(int mode, MeshComposer *self);
	static void next(int index, MeshComposer *self);
	static void end(MeshComposer *self);
	static void error(int code);
};

#endif // file

