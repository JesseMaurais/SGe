#ifndef __Mesh__
#define __Mesh__

#include "Vector.hpp"
#include "Matrix.hpp"
#include "Plane.hpp"
#include <vector>

const int Knot = -1;

struct Triangle
{
	Vector normal, vertexes[3];
};

union Surface
{
	int points[3], edges[3];
};

struct Point
{
	int vertex, normal, texCoord;
};

struct Mesh
{
	std::vector<Point> points;
	std::vector<Surface> faces;
	std::vector<Vector> normals;
	std::vector<Vector> vertexes;
	std::vector<Vector> texCoords;

	void Clear();

	int AddSurface(Surface);
	int AddPoint(Point);

	int AddVertex(Vector);
	int AddTexCoord(Vector);
	int AddNormal(Vector);

	int NewVertex(int a, int b, double cut);
	int NewTexCoord(int a, int b, double cut);
	int NewNormal(int a, int b, double cut);

	Vector &GetVertex(int index);
	Vector &GetTexCoord(int index);
	Vector &GetNormal(int index);

	void GetVertexes(int *indexes, Vector *V, int n=3);
	void GetTexCoords(int *indexes, Vector *V, int n=3);
	void GetNormals(int *indexes, Vector *V, int n=3);

	void GetVertexes(int index, Vector *V);
	void GetTexCoords(int index, Vector *V);
	void GetNormals(int index, Vector *V);

	Plane GetPlane(int index);
	Triangle GetTriangle(int index);
	Matrix GetTangentSpace(int index);
};

struct MeshComposer : virtual Mesh
{
	int Next(int index);
	void Begin(int type);
	void End();
	
 private:
 
 	int mode, counter;
};

#endif // file

