#ifndef __Geom__
#define __Geom__

#include "ODE.hpp"
#include "VBO.hpp"
#include "Model.hpp"

struct Geometry : virtual Model, Geom, VBO
{
	~Geometry();

	void Update();

	dBodyID body;
	dGeomID geom;

	void Rotation(double radian, double x, double y, double z);
	void Position(double x, double y, double z);
	void LinearVel(double x, double y, double z);
	void AngularVel(double x, double y, double z);
};

struct Box : Geometry
{
	Box();
};

#endif // file

