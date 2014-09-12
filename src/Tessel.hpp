#ifndef __Tessel__
#define __Tessel__

#include "OpenGL.hpp"
#include "Mesh.hpp"

struct Tessel : virtual MeshComposer
{
	Tessel();
	~Tessel();

	void Vertex(int index, double *v);
	void Vertex(Vector);
	void BeginPolygon();
	void Contour();
	void EndPolygon();

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

