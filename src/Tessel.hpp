#ifndef Tessel_hpp
#define Tessel_hpp

#include "Mesh.hpp"

class GLUtesselator;

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
};

#endif // file

