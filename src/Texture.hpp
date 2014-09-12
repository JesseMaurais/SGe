#ifndef __Texture__
#define __Texture__

#include "OpenGL.hpp"

void LoadCubeMap(const char *dir, const char *ext="png");
void CubeTexParameters(GLenum type);
void NormalCubeMap(int power);
void DrawCubeMap(void);

#endif // file
