#include "Mesh.hpp"

#include "std.hpp"

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
	M.groups.swap(groups);
	M.edges.swap(edges);
	M.points.swap(points);
	M.faces.swap(faces);
	M.colors.swap(colors);
	M.normals.swap(normals);
	M.vertexes.swap(vertexes);
	M.texCoords.swap(texCoords);
}

void Mesh::Append(Mesh const &M)
{
	std::size_t size = faces.size();
	const auto end = M.groups.end();
	for (auto it = M.groups.begin(); it != end; ++it)
	{
		Group &G = groups[it->first];
		G.first = it->second.first + to_int(size);
		G.count = it->second.count;
	}

	stl::merge(M.edges, edges);
	stl::merge(M.points, points);
	stl::merge(M.faces, faces);
	stl::merge(M.colors, colors);
	stl::merge(M.normals, normals);
	stl::merge(M.vertexes, vertexes);
	stl::merge(M.texCoords, texCoords);
}

int Mesh::AddSurface(Surface const &S)
{
	std::size_t index = faces.size();
	faces.push_back(S);
	return to_int(index);
}

int Mesh::AddEdge(Edge const &E)
{
	std::size_t index = edges.size();
	edges.push_back(E);
	return to_int(index);
}

int Mesh::AddPoint(Point const &P)
{
	std::size_t index = points.size();
	points.push_back(P);
	return ~ to_int(index);
}

int Mesh::AddVertex(Vector const &V)
{
	std::size_t index = vertexes.size();
	vertexes.push_back(V);
	return to_int(index);
}

int Mesh::AddTexCoord(Vector const &T)
{
	std::size_t index = texCoords.size();
	texCoords.push_back(T);
	return to_int(index);
}

int Mesh::AddNormal(Vector const &N)
{
	std::size_t index = normals.size();
	normals.push_back(N);
	return to_int(index);
}

int Mesh::AddColor(Vector const &C)
{
	std::size_t index = colors.size();
	colors.push_back(C);
	return to_int(index);
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
	std::size_t size = Mesh::faces.size();
	G.first = to_int(size);
	return G.first;
}

int MeshComposer::EndGroup()
{
	Group &G = Mesh::groups[group];
	std::size_t size = Mesh::faces.size();
	G.count = to_int(size) - G.first;
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

int MeshComposer::Next(Point const &P)
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

