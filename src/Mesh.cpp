#include "Mesh.hpp"
#include "STL.hpp"

void Mesh::Clear()
{
	edges.clear();
	points.clear();
	faces.clear();
	groups.clear();
	colors.clear();
	normals.clear();
	vertexes.clear();
	texCoords.clear();
}

void Mesh::Swap(Mesh &M)
{
	groups.swap(M.groups);
	edges.swap(M.edges);
	points.swap(M.points);
	faces.swap(M.faces);
	colors.swap(M.colors);
	normals.swap(M.normals);
	vertexes.swap(M.vertexes);
	texCoords.swap(M.texCoords);
}

int Mesh::AddSurface(Surface S)
{
	using namespace stl;
	std::size_t index = faces.size();
	faces.push_back(S);
	return as<int>(index);
}

int Mesh::AddEdge(Edge E)
{
	std::size_t index = edges.size();
	edges.push_back(E);
	return stl::as_int(index);
}

int Mesh::AddPoint(Point P)
{
	std::size_t index = points.size();
	points.push_back(P);
	return ~ stl::as_int(index);
}

int Mesh::AddVertex(Vector V)
{
	std::size_t index = vertexes.size();
	vertexes.push_back(V);
	return stl::as_int(index);
}

int Mesh::AddTexCoord(Vector T)
{
	std::size_t index = texCoords.size();
	texCoords.push_back(T);
	return stl::as_int(index);
}

int Mesh::AddNormal(Vector N)
{
	std::size_t index = normals.size();
	normals.push_back(N);
	return stl::as_int(index);
}

int Mesh::AddColor(Vector C)
{
	std::size_t index = colors.size();
	colors.push_back(C);
	return stl::as_int(index);
}

int Mesh::NewPoint(int a, int b, Scalar c)
{
	Point P;
	P.vertex = NewVertex(a, b, c);
	P.texCoord = NewTexCoord(a, b, c);
	P.normal = NewNormal(a, b, c);
	P.color = NewColor(a, b, c);
	return AddPoint(P);
}

int Mesh::NewVertex(int a, int b, Scalar c)
{
	Vector &A = GetVertex(a);
	Vector &B = GetVertex(b);

	return AddVertex(A*c + B*(1-c));
}

int Mesh::NewTexCoord(int a, int b, Scalar c)
{
	Vector &A = GetTexCoord(a);
	Vector &B = GetTexCoord(b);

	return AddTexCoord(A*c + B*(1-c));
}

int Mesh::NewNormal(int a, int b, Scalar c)
{
	Vector &A = GetNormal(a);
	Vector &B = GetNormal(b);

	return AddNormal(A*c + B*(1-c));
}

int Mesh::NewColor(int a, int b, Scalar c)
{
	Vector &A = GetColor(a);
	Vector &B = GetColor(b);

	return AddColor(A*c + B*(1-c));
}

Mesh::Point &Mesh::GetPoint(int index)
{
	if (index < 0)
	{
		return points.at(~index);
	}
	static Point point;
	point.vertex = index;
	point.texCoord = index;
	point.normal = index;
	point.color = index;
	return point;
}

Vector &Mesh::GetVertex(int index)
{
	return vertexes.at(index < 0 ? points.at(~index).vertex : index);
}

Vector &Mesh::GetTexCoord(int index)
{
	return texCoords.at(index < 0 ? points.at(~index).texCoord : index);
}

Vector &Mesh::GetNormal(int index)
{
	return normals.at(index < 0 ? points.at(~index).normal : index);
}

Vector &Mesh::GetColor(int index)
{
	return colors.at(index < 0 ? points.at(~index).color : index);
}

void Mesh::GetPoints(int *indexes, Point *P, int n)
{
	for (int i = 0; i < n; ++i) P[i] = GetPoint(indexes[i]);
}

void Mesh::GetVertexes(int *indexes, Vector *V, int n)
{
	for (int i = 0; i < n; ++i) V[i] = GetVertex(indexes[i]);
}

void Mesh::GetTexCoords(int *indexes, Vector *V, int n)
{
	for (int i = 0; i < n; ++i) V[i] = GetTexCoord(indexes[i]);
}

void Mesh::GetNormals(int *indexes, Vector *V, int n)
{
	for (int i = 0; i < n; ++i) V[i] = GetNormal(indexes[i]);
}

void Mesh::GetColors(int *indexes, Vector *V, int n)
{
	for (int i = 0; i < n; ++i) V[i] = GetColor(indexes[i]);
}

void Mesh::GetPoints(int index, Point *P)
{
	GetPoints(faces[index].points, P);
}

void Mesh::GetVertexes(int index, Vector *V)
{
	GetVertexes(faces[index].points, V);
}

void Mesh::GetTexCoords(int index, Vector *V)
{
	GetTexCoords(faces[index].points, V);
}

void Mesh::GetNormals(int index, Vector *V)
{
	GetNormals(faces[index].points, V);
}

void Mesh::GetColors(int index, Vector *V)
{
	GetColors(faces[index].points, V);
}

Plane Mesh::GetPlane(int index)
{
	Triangle T;
	T = GetTriangle(index);
	return Plane(T.normal, T.vertexes[0]);
}

Mesh::Triangle Mesh::GetTriangle(int index)
{
	Triangle T;
	GetVertexes(index, T.vertexes);
	T.normal.Right(T.vertexes[0], T.vertexes[1], T.vertexes[2]);
	return T;
}

Matrix Mesh::GetTangentSpace(int index)
{
	Vector V [3];
	Vector T [3];
	
	GetVertexes(index, V);
	GetTexCoords(index, T);

	Matrix TBN;
	TBN.Tangent(V, T);
	return TBN;
}

int MeshComposer::BeginGroup(int id)
{
	group = id;
	Group &G = Mesh::groups[group];
	G.first = Mesh::faces.size();
	return G.first;
}

int MeshComposer::EndGroup()
{
	Group &G = Mesh::groups[group];
	G.count = Mesh::faces.size() - G.first;
	return G.count;
}

int MeshComposer::Begin(Compose type)
{
	mode = type;
	counter = 0;
	return faces.size();
}

int MeshComposer::End()
{
	return faces.size();
}

int MeshComposer::Vertex(Scalar x, Scalar y, Scalar z)
{
	return Mesh::AddVertex(Vector(x, y, z));
}

int MeshComposer::TexCoord(Scalar x, Scalar y, Scalar z)
{
	return Mesh::AddTexCoord(Vector(x, y, z));
}

int MeshComposer::Normal(Scalar x, Scalar y, Scalar z)
{
	return Mesh::AddNormal(Vector(x, y, z));
}

int MeshComposer::Color(Scalar x, Scalar y, Scalar z)
{
	return Mesh::AddColor(Vector(x, y, z));
}

int MeshComposer::Next(Point P)
{
	return Next(~ Mesh::AddPoint(P));
}

int MeshComposer::Next(int index)
{
	static Surface S;
	const int count = counter++;
	int n;

	switch (mode)
	{
	case Triangles:
	 
		n = count % 3;
		S.points[n] = index;
		if (n == 2)
		{
		 return AddSurface(S);
		}
	 	break;

	case Fan:
	 
		if (count > 2)
		{
		 S.points[1] = S.points[2];
		 S.points[2] = index;
		 return AddSurface(S);
		}
		else
		{
		 S.points[count] = index;

		 if (count == 2)
		 {
		  return AddSurface(S);
		 }
		}
	 	break;

	case Strip:
	 
		if (count > 2)
		{
		 S.points[0] = S.points[1];
		 S.points[1] = S.points[2];
		 S.points[2] = index;
		 return AddSurface(S);
		}
		else
		{
		 S.points[count] = index;
		 if (count == 2)
		 {
		  return AddSurface(S);
		 }
		}
	 	break;

	case Quads:
	 
		n = count % 4;
		if (n > 2)
		{
		 S.points[1] = index;
		 index = S.points[0];
		 S.points[0] = S.points[2];
		 S.points[2] = index;
		 return AddSurface(S);
		}
		else
		{
		 S.points[n] = index;
		 if (n == 2)
		 {
		  return AddSurface(S);
		 }
		}
		break;
	}
	return Knot;
}

