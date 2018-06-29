#include "Texture.hpp"
#include "Surface.hpp"
#include "Vector.hpp"
#include "OpenGL.hpp"
#include "SDL.hpp"
#include "io.hpp"
#include <cstdio>

static signed LoadCubeFace(int face, const char *path)
{
	SDL_Surface *image = IMG_Load(path);
	if (!image)
	{
		SDL_perror("IMG_Load");
		return -1;
	}
	signed error = LoadCubeMap(image, face, GL_RGB);
	SDL_FreeSurface(image);
	return error;
}

void LoadCubeMap(const char *dir, const char *ext)
{
	std::string path;
	CubeTexParameters(GL_TEXTURE_CUBE_MAP);

	io::sprintf(path, "{1}/left.{2}", dir, ext);
	LoadCubeFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, path);

	io::sprintf(path, "{1}/right.{2}", dir, ext);
	LoadCubeFace(GL_TEXTURE_CUBE_MAP_POSITIVE_X, path);

	io::sprintf(path, "{1}/bottom.{2}", dir, ext);
	LoadCubeFace(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, path);

	io::sprintf(path, "{1}/top.{2}", dir, ext);
	LoadCubeFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, path);

	io::sprintf(path, "{1}/back.{[2}", dir, ext);
	LoadCubeFace(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, path);

	io::sprintf(path, "{1}/front.{2}", dir, ext);
	LoadCubeFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, path);
}

void CubeTexParameters(unsigned type)
{
	// Filters
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Wrapping
	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

static void CubeMapVertex(signed x, signed y, signed z)
{
	glTexCoord3f(x, y, z); glVertex3f(x, y, z);
}

void DrawCubeMap(void)
{
	glBegin(GL_QUADS);
	 // left
	 CubeMapVertex(-1,-1,+1);
	 CubeMapVertex(-1,-1,-1);
	 CubeMapVertex(-1,+1,-1);
	 CubeMapVertex(-1,+1,+1);
	 // right
	 CubeMapVertex(+1,-1,-1);
	 CubeMapVertex(+1,-1,+1);
	 CubeMapVertex(+1,+1,+1);
	 CubeMapVertex(+1,+1,-1);
	 // bottom
	 CubeMapVertex(+1,-1,+1);
	 CubeMapVertex(-1,-1,+1);
	 CubeMapVertex(-1,-1,-1);
	 CubeMapVertex(+1,-1,-1);
	 //top
	 CubeMapVertex(+1,+1,-1);
	 CubeMapVertex(-1,+1,-1);
	 CubeMapVertex(-1,+1,+1);
	 CubeMapVertex(+1,+1,+1);
	 // front
	 CubeMapVertex(+1,-1,+1);
	 CubeMapVertex(-1,-1,+1);
	 CubeMapVertex(-1,+1,+1);
	 CubeMapVertex(+1,+1,+1);
	 // back
	 CubeMapVertex(-1,-1,-1);
	 CubeMapVertex(+1,-1,-1);
	 CubeMapVertex(+1,+1,-1);
	 CubeMapVertex(-1,+1,-1);
	 // done
	glEnd();
}

static void CubeMapPackPixel(char RGB[3], double x, double y, double z)
{
	Vector V(x, y, z);
	V.Normalize();
	V = V / 2.0;
	V = V + 0.5;

	RGB[0] = int(V.R * 255);
	RGB[1] = int(V.G * 255);
	RGB[2] = int(V.B * 255);
}

void NormalCubeMap(unsigned power)
{
	unsigned size = 1 << power;
	unsigned half = size >> 1;

	char map[6][size][size][3];

	for (unsigned i = 0; i < size; ++i)
	{
		double v = 0.5 + i - half;

		for (unsigned j = 0; j < size; ++j)
		{
			double u = 0.5 + j - half;

			CubeMapPackPixel(map[0][i][j], +half, -v, -u);
			CubeMapPackPixel(map[1][i][j], -half, -v, +u);
			CubeMapPackPixel(map[2][i][j], +u, +half, +v);
			CubeMapPackPixel(map[3][i][j], +u, -half, -v);
			CubeMapPackPixel(map[4][i][j], +u, -v, +half);
			CubeMapPackPixel(map[5][i][j], -u, -v, -half);
		}
	}

	CubeTexParameters(GL_TEXTURE_CUBE_MAP);

	glTexImage2D
	(GL_TEXTURE_CUBE_MAP_POSITIVE_X
	,0
	,GL_RGB
	,size
	,size
	,0
	,GL_RGB
	,GL_UNSIGNED_BYTE
	,map[0]
	);

	glTexImage2D
	(GL_TEXTURE_CUBE_MAP_NEGATIVE_X
	,0
	,GL_RGB
	,size
	,size
	,0
	,GL_RGB
	,GL_UNSIGNED_BYTE
	,map[1]
	);

	glTexImage2D
	(GL_TEXTURE_CUBE_MAP_POSITIVE_Y
	,0
	,GL_RGB
	,size
	,size
	,0
	,GL_RGB
	,GL_UNSIGNED_BYTE
	,map[2]
	);

	glTexImage2D
	(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
	,0
	,GL_RGB
	,size
	,size
	,0
	,GL_RGB
	,GL_UNSIGNED_BYTE
	,map[3]
	);

	glTexImage2D
	(GL_TEXTURE_CUBE_MAP_POSITIVE_Z
	,0
	,GL_RGB
	,size
	,size
	,0
	,GL_RGB
	,GL_UNSIGNED_BYTE
	,map[4]
	);

	glTexImage2D
	(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	,0
	,GL_RGB
	,size
	,size
	,0
	,GL_RGB
	,GL_UNSIGNED_BYTE
	,map[5]
	);
}

