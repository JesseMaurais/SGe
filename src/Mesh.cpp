#include "Mesh.hpp"
#include "OpenGL.hpp"

void Mesh::Clear()
{
	points.clear();
	faces.clear();
	colors.clear();
	normals.clear();
	vertexes.clear();
	texCoords.clear();
}

void Mesh::Copy(Mesh &M)
{
	groups = M.groups;
	points = M.points;
	faces = M.faces;
	colors = M.colors;
	normals = M.normals;
	vertexes = M.vertexes;
	texCoords = M.texCoords;
}

void Mesh::Swap(Mesh &M)
{
	groups.swap(M.groups);
	points.swap(M.points);
	faces.swap(M.faces);
	colors.swap(M.colors);
	normals.swap(M.normals);
	vertexes.swap(M.vertexes);
	texCoords.swap(M.texCoords);
}

int Mesh::AddGroup(Segment G)
{
	int index = groups.size();
	groups.push_back(G);
	return index;
}

int Mesh::AddSurface(Surface S)
{
	int index = faces.size();
	faces.push_back(S);
	return index;
}

int Mesh::AddPoint(Point P)
{
	int index = points.size();
	points.push_back(P);
	return ~index;
}

int Mesh::AddVertex(Vector V)
{
	int index = vertexes.size();
	vertexes.push_back(V);
	return index;
}

int Mesh::AddTexCoord(Vector T)
{
	int index = texCoords.size();
	texCoords.push_back(T);
	return index;
}

int Mesh::AddNormal(Vector N)
{
	int index = normals.size();
	normals.push_back(N);
	return index;
}

int Mesh::AddColor(Vector C)
{
	int index = colors.size();
	colors.push_back(C);
	return index;
}

int Mesh::NewVertex(int a, int b, double c)
{
	Vector &A = GetVertex(a);
	Vector &B = GetVertex(b);

	return AddVertex(c*A + (1 - c)*B);
}

int Mesh::NewTexCoord(int a, int b, double c)
{
	Vector &A = GetTexCoord(a);
	Vector &B = GetTexCoord(b);

	return AddTexCoord(c*A + (1 - c)*B);
}

int Mesh::NewNormal(int a, int b, double c)
{
	Vector &A = GetNormal(a);
	Vector &B = GetNormal(b);

	return AddNormal(c*A + (1 - c)*B);
}

int Mesh::NewColor(int a, int b, double c)
{
	Vector &A = GetColor(a);
	Vector &B = GetColor(b);

	return AddColor(c*A + (1 - c)*B);
}

Vector &Mesh::GetVertex(int index)
{
	return vertexes.at(index < 0 ? points[~index].vertex : index);
}

Vector &Mesh::GetTexCoord(int index)
{
	return texCoords.at(index < 0 ? points[~index].texCoord : index);
}

Vector &Mesh::GetNormal(int index)
{
	return normals.at(index < 0 ? points[~index].normal : index);
}

Vector &Mesh::GetColor(int index)
{
	return colors.at(index < 0 ? points[~index].color : index);
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

void Mesh::GetColor(int index, Vector *V)
{
	GetColors(faces[index].points, V);
}

Plane Mesh::GetPlane(int index)
{
	Triangle T;
	T = GetTriangle(index);
	return Plane(T.normal, T.vertexes[0]);
}

Triangle Mesh::GetTriangle(int index)
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
}

int MeshComposer::EndGroup()
{
	Group &G = Mesh::groups[group];
	G.count = Mesh::faces.size() - G.first;
	return group;
}

int MeshComposer::Begin(int type)
{
	mode = type;
	counter = 0;
	return faces.size();
}

int MeshComposer::End()
{
	mode = Knot;
	return faces.size();
}

int MeshComposer::Vertex(double x, double y, double z)
{
	return Mesh::AddVertex(Vector(x, y, z));
}

int MeshComposer::TexCoord(double x, double y, double z)
{
	return Mesh::AddTexCoord(Vector(x, y, z));
}

int MeshComposer::Normal(double x, double y, double z)
{
	return Mesh::AddNormal(Vector(x, y, z));
}

int MeshComposer::Color(double x, double y, double z)
{
	return Mesh::AddColor(Vector(x, y, z));
}

int MeshComposer::Next(int index)
{
	static Surface S;
	const int count = counter++;
	int n;

	switch (mode)
	{
	case GL_TRIANGLES:
	 
		n = count % 3;
		S.points[n] = index;
		if (n == 2)
		{
		 return AddSurface(S);
		}
	 	break;

	case GL_TRIANGLE_FAN:
	 
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

	case GL_TRIANGLE_STRIP:
	case GL_QUAD_STRIP:
	 
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

	case GL_QUADS:
	 
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

