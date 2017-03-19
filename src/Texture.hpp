#ifndef __Texture__
#define __Texture__

void LoadCubeMap(const char *dir, const char *ext="png");
void CubeTexParameters(unsigned type);
void NormalCubeMap(unsigned power);
void DrawCubeMap(void);

#endif // file
