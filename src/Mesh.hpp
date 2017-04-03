#ifndef Mesh_hpp
#define Mesh_hpp

#include <map>
#include <vector>
#include "Plane.hpp"
#include "Matrix.hpp"

struct Mesh
{
	static constexpr int Knot = ~0;

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

	std::vector<Edge> edges;
	std::vector<Point> points;
	std::vector<Surface> faces;
	std::map<int, Group> groups;
	std::vector<Vector> vertexes;
	std::vector<Vector> texCoords;
	std::vector<Vector> normals;
	std::vector<Vector> colors;

	void Clear();
	void Swap(Mesh &mesh);
	void Merge(Mesh const &mesh);

	int AddSurface(Surface const &S);
	int AddEdge(Edge const &E);
	int AddPoint(Point const &P);
	int AddVertex(Vector const &V);
	int AddTexCoord(Vector const &T);
	int AddNormal(Vector const &N);
	int AddColor(Vector const &C);

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
	void GetTexCoords(int *indexes, Vector *T, int n=3);
	void GetNormals(int *indexes, Vector *N, int n=3);
	void GetColors(int *indexes, Vector *C, int n=3);

	void GetPoints(int index, Point *P);
	void GetVertexes(int index, Vector *V);
	void GetTexCoords(int index, Vector *T);
	void GetNormals(int index, Vector *N);
	void GetColors(int index, Vector *C);

	Plane GetPlane(int index);
	Matrix GetTangentSpace(int index);
	Triangle GetTriangle(int index);
};

struct MeshComposer : virtual Mesh
{
	enum Compose { Triangles, Quads, Strip, Fan };

	int BeginGroup(int id);
	int EndGroup();

	int Begin(Compose);
	int End();

	int Vertex(Scalar x, Scalar y, Scalar z);
	int TexCoord(Scalar x, Scalar y, Scalar z);
	int Normal(Scalar x, Scalar y, Scalar z);
	int Color(Scalar x, Scalar y, Scalar z);

	int Next(Point const &P);
	int Next(int point);
	
 private:
 
	Compose mode;
 	int group, counter;
};

#endif // file

