#include "Obj.hpp"
#include <cassert>
#include <cstring>
#include <cstdio>
#include <cctype>

bool ModelObj::Load(const char *path)
{
	FILE *file = fopen(path, "r");
	if (file)
	{
		char buffer[BUFSIZ];
		while (!feof(file))
		{
			char *line = fgets(buffer, BUFSIZ, file);
			if (!line) continue;
			
			while (isspace(*line)) ++line;
			if ('#' == *line) continue;
			
			if (!strncmp(line, "vn", 2))
				ReadNormal(line);
			else
			if (!strncmp(line, "vt", 2))
				ReadTexCoord(line);
			else
			if (!strncmp(line, "v", 1))
				ReadVertex(line);
			else
			if (!strncmp(line, "f", 1))
				ReadFace(line);
			/*
			else
			if (!strncmp(line, "g", 1))
				fprintf(stderr, "%s", line); //TODO
			else
			if (!strncmp(line, "usemtl", 6))
				fprintf(stderr, "%s", line); //TODO
			else
			if (!strncmp(line, "mtllib", 6))
				fprintf(stderr, "%s", line); //TODO
			*/
		}
		fclose(file);
	}
	else perror(path);
	return file;
}

int ModelObj::ReadVertex(char *line)
{
	Vector V;
	sscanf(line, "v%*[ \t]%lf%*[ \t]%lf%*[ \t]%lf", &V.x, &V.z, &V.y);
	return AddVertex(V);
}

int ModelObj::ReadTexCoord(char *line)
{
	Vector V;
	sscanf(line, "vt%*[ \t]%lf%*[ \t]%lf", &V.s, &V.t);
	return AddTexCoord(V);	
}

int ModelObj::ReadNormal(char *line)
{
	Vector V;
	sscanf(line, "vn%*[ \t]%lf%*[ \t]%lf%*[ \t]%lf", &V.i, &V.k, &V.j);
	return AddNormal(V);
}

int ModelObj::ReadPoint(char *it)
{
	Point P;
	char *a;
	
	if (strstr(it, "//"))
	 sscanf(it, "%d//%d", &P.vertex, &P.normal);
	else
	if (!(a = strchr(it, '/')))
	 sscanf(it, "%d", &P.vertex);
	else
	if (strrchr(it, '/') == a)
	 sscanf(it, "%d/%d", &P.vertex, &P.texCoord);
	else
	 sscanf(it, "%d/%d/%d", &P.vertex, &P.texCoord, &P.normal);
	
	P.vertex += 0 > P.vertex ? vertexes.size() : Knot;
	P.texCoord += 0 > P.texCoord ? texCoords.size() : Knot;
	P.normal += 0 > P.normal ? normals.size() : Knot;
	
	return AddPoint(P);
}

int ModelObj::ReadFace(char *line)
{
	const char *delim = " \t\n";
	line = strtok(line, delim);
	
	Begin(GL_POLYGON);
	
	while (line = strtok(0, delim))
	{
	 signed point = ReadPoint(line);
	 Next(point);
	}
	
	End();
}

