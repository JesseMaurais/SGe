#ifndef __Geom__
#define __Geom__

#include "Model.hpp"
#include "Ode.hpp"
#include "Render.hpp"

struct Geometry : virtual Model, Geom, Render
{
	~Geometry();

	void Update();

	dBodyID body;
	dGeomID geom;

	void Rotation(Scalar radian, Scalar x, Scalar y, Scalar z);
	void Position(Scalar x, Scalar y, Scalar z);
	void LinearVel(Scalar x, Scalar y, Scalar z);
	void AngularVel(Scalar x, Scalar y, Scalar z);

	void Cube(Scalar size);
	void Sphere(int depth);
};


#endif // file

