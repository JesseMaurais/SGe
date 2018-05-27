#ifndef Texture_hpp
#define Texture_hpp

void LoadCubeMap(const char *dir, const char *ext="png");
void CubeTexParameters(unsigned type);
void NormalCubeMap(unsigned power);
void DrawCubeMap(void);

#endif // file
