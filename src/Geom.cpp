#include "Matrix.hpp"
#include "Quaternion.hpp"
#include "OpenGL.hpp"
#include "Geom.hpp"
#include "ODE.hpp"
#include "Lua.hpp"

Geometry::~Geometry()
{
	dBodyDestroy(body);
	dGeomDestroy(geom);
}

void Geometry::Update()
{
	const dReal *R = dGeomGetRotation(geom);
	const dReal *P = dGeomGetPosition(geom);

	GLdouble M[16];

	M[ 0] = R[ 0]; M[ 1] = R[ 4]; M[ 2] = R[ 8]; M[ 3] = 0.0;
	M[ 4] = R[ 1]; M[ 5] = R[ 5]; M[ 6] = R[ 9]; M[ 7] = 0.0;
	M[ 8] = R[ 2]; M[ 9] = R[ 6]; M[10] = R[10]; M[11] = 0.0;
	M[12] = P[ 0]; M[13] = P[ 1]; M[14] = P[ 2]; M[15] = 1.0;

	glPushMatrix();
	glMultMatrixd(M);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3d(0.5, 0.5, 0.5);
	Render::Draw();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3d(0.8, 0.0, 0.0);
	Render::Draw();
	glPopMatrix();
}

void Geometry::Rotation(Scalar radian, Scalar x, Scalar y, Scalar z)
{
	Quaternion Q;
	Q.Rotate(radian, x, y, z);
	dBodySetQuaternion(body, Q.v);
}

void Geometry::Position(Scalar x, Scalar y, Scalar z)
{
	dBodySetPosition(body, x, y, z);
}

void Geometry::LinearVel(Scalar x, Scalar y, Scalar z)
{
	dBodySetLinearVel(body, x, y, z);
}

void Geometry::AngularVel(Scalar x, Scalar y, Scalar z)
{
	dBodySetAngularVel(body, x, y, z);
}

void Geometry::Cube(Scalar l)
{
	BeginGroup(0);
	Model::Cube(l);
	EndGroup();

	dMass mass;
	mass.setBox(0.1, l, l, l);
	body = dBodyCreate(World);
	dBodySetMass(body, &mass);
	geom = dCreateBox(Space, l, l, l);
	dGeomSetData(geom, this);
	dGeomSetBody(geom, body);
}

