#include "Mesh.hpp"
#include "OpenGL.hpp"


void Mesh::Clear()
{
	faces.clear();
	points.clear();
	normals.clear();
	vertexes.clear();
	texCoords.clear();
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

int Mesh::AddTexCoord(Vector C)
{
	int index = texCoords.size();
	texCoords.push_back(C);
	return index;
}

int Mesh::AddNormal(Vector N)
{
	int index = normals.size();
	normals.push_back(N);
	return index;
}

int Mesh::NewVertex(int a, int b, double cut)
{
	Vector &A = GetVertex(a);
	Vector &B = GetVertex(b);

	return AddVertex(A + (B - A)*cut);
}

int Mesh::NewTexCoord(int a, int b, double cut)
{
	Vector &A = GetTexCoord(a);
	Vector &B = GetTexCoord(b);

	return AddTexCoord(A + (B - A)*cut);
}

int Mesh::NewNormal(int a, int b, double cut)
{
	Vector &A = GetNormal(a);
	Vector &B = GetNormal(b);

	return AddNormal(A*cut + B*(1-cut));
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
	Vector C [3];
	
	GetVertexes(index, V);
	GetTexCoords(index, C);

	Matrix TBN;
	TBN.Tangent(V, C);
	return TBN;
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

void MeshComposer::Begin(int type)
{
	mode = type;
	counter = 0;
}

void MeshComposer::End()
{
	mode = Knot;
}



