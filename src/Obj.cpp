#include "Obj.hpp"
#include <cassert>
#include <cstring>
#include <cstdio>
#include <cctype>

bool ModelObj::Load(const char *path)
{
	std::FILE *file = std::fopen(path, "r");
	if (file)
	{
		char buffer[BUFSIZ];
		while (not std::feof(file))
		{
			char *line = std::fgets(buffer, BUFSIZ, file);
			if (!line) continue;
			
			while (std::isspace(*line)) ++line;
			if ('#' == *line) continue;
			
			if (not std::strncmp(line, "vn", 2))
				ReadNormal(line);
			else
			if (not std::strncmp(line, "vt", 2))
				ReadTexCoord(line);
			else
			if (not std::strncmp(line, "v", 1))
				ReadVertex(line);
			else
			if (not std::strncmp(line, "f", 1))
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
		std::fclose(file);
	}
	else std::perror(path);
	return file;
}

int ModelObj::ReadVertex(char *line)
{
	Vector V;
	std::sscanf(line, "v%*[ \t]%lf%*[ \t]%lf%*[ \t]%lf", &V.x, &V.z, &V.y);
	return AddVertex(V);
}

int ModelObj::ReadTexCoord(char *line)
{
	Vector V;
	std::sscanf(line, "vt%*[ \t]%lf%*[ \t]%lf", &V.s, &V.t);
	return AddTexCoord(V);	
}

int ModelObj::ReadNormal(char *line)
{
	Vector V;
	std::sscanf(line, "vn%*[ \t]%lf%*[ \t]%lf%*[ \t]%lf", &V.i, &V.k, &V.j);
	return AddNormal(V);
}

int ModelObj::ReadPoint(char *it)
{
	Point P;
	char *a;
	
	if (std::strstr(it, "//"))
		std::sscanf(it, "%d//%d", &P.vertex, &P.normal);
	else
	if (not (a = std::strchr(it, '/')))
		std::sscanf(it, "%d", &P.vertex);
	else
	if (std::strrchr(it, '/') == a)
		std::sscanf(it, "%d/%d", &P.vertex, &P.texCoord);
	else
		std::sscanf(it, "%d/%d/%d", &P.vertex, &P.texCoord, &P.normal);
	
	P.vertex += 0 > P.vertex ? vertexes.size() : Knot;
	P.texCoord += 0 > P.texCoord ? texCoords.size() : Knot;
	P.normal += 0 > P.normal ? normals.size() : Knot;
	
	return AddPoint(P);
}

int ModelObj::ReadFace(char *line)
{
	const char *delim = " \t\n";
	line = std::strtok(line, delim);
	
	Begin(GL_POLYGON);
	
	while ((line = std::strtok(0, delim)))
	{
	 signed point = ReadPoint(line);
	 Next(point);
	}
	
	End();
	return Knot;
}

