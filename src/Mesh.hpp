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

struct Edge
{
	int points[2];
};

struct Mesh
{
	std::vector<Edge> edges;
	std::vector<Point> points;
	std::vector<Surface> faces;
	std::map<int, Group> groups;
	std::vector<Vector> vertexes;
	std::vector<Vector> texCoords;
	std::vector<Vector> normals;
	std::vector<Vector> colors;

	void Clear();
	void Copy(Mesh &);
	void Swap(Mesh &);

	int AddSurface(Surface);
	int AddEdge(Edge);
	int AddPoint(Point);
	int AddVertex(Vector);
	int AddTexCoord(Vector);
	int AddNormal(Vector);
	int AddColor(Vector);

	int NewPoint(int a, int b, Scalar cut);
	int NewVertex(int a, int b, Scalar cut);
	int NewTexCoord(int a, int b, Scalar cut);
	int NewNormal(int a, int b, Scalar cut);
	int NewColor(int a, int b, Scalar cut);

	Point  &GetPoint(int index);
	Vector &GetVertex(int index);
	Vector &GetTexCoord(int index);
	Vector &GetNormal(int index);
	Vector &GetColor(int index);

	void GetPoints(int *indexes, Point *P, int n=3);
	void GetVertexes(int *indexes, Vector *V, int n=3);
	void GetTexCoords(int *indexes, Vector *V, int n=3);
	void GetNormals(int *indexes, Vector *V, int n=3);
	void GetColors(int *indexes, Vector *V, int n=3);

	void GetPoints(int index, Point *P);
	void GetVertexes(int index, Vector *V);
	void GetTexCoords(int index, Vector *V);
	void GetNormals(int index, Vector *V);
	void GetColors(int index, Vector *V);

	Plane GetPlane(int index);
	Matrix GetTangentSpace(int index);
	Triangle GetTriangle(int index);
};

struct MeshComposer : virtual Mesh
{
	int BeginGroup(int id);
	int EndGroup();

	int Begin(int mode);
	int End();

	int Vertex(Scalar x, Scalar y, Scalar z);
	int TexCoord(Scalar x, Scalar y, Scalar z);
	int Normal(Scalar x, Scalar y, Scalar z);
	int Color(Scalar x, Scalar y, Scalar z);

	int Next(Point P);
	int Next(int point);
	
 private:
 
 	int group, mode, counter;
};

#endif // file

