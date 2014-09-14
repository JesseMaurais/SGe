#ifndef __Mesh__
#define __Mesh__

#include "Matrix.hpp"
#include "Plane.hpp"
#include <vector>
#include <map>

const int Knot = -1;

struct Triangle
{
	Vector normal, vertexes[3];
};

struct Group
{
	int first, count;
};

union Surface
{
	int points[3], edges[3];
};

struct Point
{
	int vertex, texCoord, normal, color;
};

struct Mesh
{
	std::vector<Vector> colors;
	std::vector<Vector> normals;
	std::vector<Vector> vertexes;
	std::vector<Vector> texCoords;
	std::map<int, Group> groups;
	std::vector<Surface> faces;
	std::vector<Point> points;

	void Clear();
	void Copy(Mesh &);
	void Swap(Mesh &);

	int AddSurface(Surface);
	int AddPoint(Point);

	int AddVertex(Vector);
	int AddTexCoord(Vector);
	int AddNormal(Vector);
	int AddColor(Vector);

	int NewVertex(int a, int b, double cut);
	int NewTexCoord(int a, int b, double cut);
	int NewNormal(int a, int b, double cut);
	int NewColor(int a, int b, double cut);

	Vector &GetVertex(int index);
	Vector &GetTexCoord(int index);
	Vector &GetNormal(int index);
	Vector &GetColor(int index);

	void GetVertexes(int *indexes, Vector *V, int n=3);
	void GetTexCoords(int *indexes, Vector *V, int n=3);
	void GetNormals(int *indexes, Vector *V, int n=3);
	void GetColors(int *indexes, Vector *V, int n=3);

	void GetVertexes(int index, Vector *V);
	void GetTexCoords(int index, Vector *V);
	void GetNormals(int index, Vector *V);
	void GetColors(int index, Vector *V);

	Plane GetPlane(int index);
	Triangle GetTriangle(int index);
	Matrix GetTangentSpace(int index);
};

struct MeshComposer : virtual Mesh
{
	int BeginGroup(int id);
	int EndGroup();

	int Begin(int type);
	int End();

	int Vertex(double x, double y, double z);
	int TexCoord(double x, double y, double z);
	int Normal(double x, double y, double z);
	int Color(double x, double y, double z);

	int Next(Point P);
	int Next(int point);
	
 private:
 
 	int group, mode, counter;
};

#endif // file

