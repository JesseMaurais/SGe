#include "Height.hpp"
#include "SDL.hpp"

bool HeightMap::Load(const char *path)
{
	SDL_Surface *image = LoadSurface(path, SDL_PIXELFORMAT_RGB888);
	
	union {
	 unsigned char *data;
	 void *pixels;
	};

	pixels = image->pixels;
	index = new int *[image->h];

	for (int y = 0; y < image->h; ++y)
	{
	 index[y] = new int [image->w];

	 for (int x = 0; x < image->w; ++x)
	 {
	  double value = data[0] + data[1] + data[2];
	  double height = value / 3;

	  index[y][x] = AddVertex(Vector(x, height, y));

	  data += 3;
	 }
	}

	SDL_FreeSurface(image);
	return true;
}

void HeightMap::Free()
{
	for (int y = 0; y < height; ++y) delete index[y];
	
	delete index;
}

void HeightMap::Draw()
{
	for (int z = 0, y = 1; y < height; z = y++)
	{
	 glBegin(GL_TRIANGLE_STRIP);

	  for (int x = 0; x < width; ++x)
	  {
	   glVertex3dv(vertexes.at(index[z][x]).v);
	   glVertex3dv(vertexes.at(index[y][x]).v);
	  }

	 glEnd();
	}
}

