#include "SkyBox.hpp"
#include "Texture.hpp"
#include "OpenGL.hpp"
#include "SDL.hpp"
#include <cstdio>

static void LoadBoxFace(GLuint texture, char *path)
{
	SDL_Surface *image = IMG_Load(path);
	if (!image)
	{
	 SDL_perror("IMG_Load");
	 return;
	}
	glBindTexture(GL_TEXTURE_2D, texture);
	LoadTexture(image, 0, GL_RGB);
	SDL_FreeSurface(image);
	CubeTexParameters(GL_TEXTURE_2D);
}

void SkyBox::Load(const char *dir, const char *ext) 
{
	glGenTextures(6, maps);
	char path[FILENAME_MAX];

	sprintf(path, "%s/left.%s", dir, ext);
	LoadBoxFace(left, path);

	sprintf(path, "%s/right.%s", dir, ext);
	LoadBoxFace(right, path);

	sprintf(path, "%s/bottom.%s", dir, ext);
	LoadBoxFace(bottom, path);

	sprintf(path, "%s/top.%s", dir, ext);
	LoadBoxFace(top, path);

	sprintf(path, "%s/back.%s", dir, ext);
	LoadBoxFace(back, path);

	sprintf(path, "%s/front.%s", dir, ext);
	LoadBoxFace(front, path);
}

void SkyBox::Free(void)
{
	glDeleteTextures(6, maps);
}

void SkyBox::Draw(void)
{
	glBindTexture(GL_TEXTURE_2D, left);
	glBegin(GL_QUADS);
	glTexCoord2d(0,0);glVertex3d(-1,-1,+1);
	glTexCoord2d(1,0);glVertex3d(-1,-1,-1);
	glTexCoord2d(1,1);glVertex3d(-1,+1,-1);
	glTexCoord2d(0,1);glVertex3d(-1,+1,+1);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, right);
	glBegin(GL_QUADS);
	glTexCoord2d(0,0);glVertex3d(+1,-1,-1);
	glTexCoord2d(1,0);glVertex3d(+1,-1,+1);
	glTexCoord2d(1,1);glVertex3d(+1,+1,+1);
	glTexCoord2d(0,1);glVertex3d(+1,+1,-1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, bottom);
	glBegin(GL_QUADS);
	glTexCoord2d(0,0);glVertex3d(-1,-1,+1);
	glTexCoord2d(1,0);glVertex3d(+1,-1,+1);
	glTexCoord2d(1,1);glVertex3d(+1,-1,-1);
	glTexCoord2d(0,1);glVertex3d(-1,-1,-1);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, top);
	glBegin(GL_QUADS);
	glTexCoord2d(0,0);glVertex3d(-1,+1,-1);
	glTexCoord2d(1,0);glVertex3d(+1,+1,-1);
	glTexCoord2d(1,1);glVertex3d(+1,+1,+1);
	glTexCoord2d(0,1);glVertex3d(-1,+1,+1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, back);
	glBegin(GL_QUADS);
	glTexCoord2d(0,0);glVertex3d(-1,-1,-1);
	glTexCoord2d(1,0);glVertex3d(+1,-1,-1);
	glTexCoord2d(1,1);glVertex3d(+1,+1,-1);
	glTexCoord2d(0,1);glVertex3d(-1,+1,-1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, front);
	glBegin(GL_QUADS);
	glTexCoord2d(0,0);glVertex3d(+1,-1,+1);
	glTexCoord2d(1,0);glVertex3d(-1,-1,+1);
	glTexCoord2d(1,1);glVertex3d(-1,+1,+1);
	glTexCoord2d(0,1);glVertex3d(+1,+1,+1);
	glEnd();
}

