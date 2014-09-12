#ifndef __SkyBox__
#define __SkyBox__

#include "OpenGL.hpp"

struct SkyBox
{
	union {
	 struct {
	  GLuint left, right, bottom, top, back, front;
	 };
	 GLuint maps[6];
	};

	void Load(const char *dir, const char *ext="jpg");
	void Free(void);
	void Draw(void);
};

#endif // file

