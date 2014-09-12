#ifndef __Shader__
#define __Shader__

#include "OpenGL.hpp"
#include "String.hpp"

struct Shader
{
	void LoadFragment(const char **paths);
	void LoadVertex(const char **paths);

	void LoadFragment(const char *path);
	void LoadVertex(const char *path);

	void LoadFragmentSource(Strings path);
	void LoadVertexSource(Strings path);

	void LinkProgram();
	void UseProgram();

	~Shader();

 protected:

	GLuint program, vertex, fragment;

 private:

	static char *Read(const char *path, int &size);
	static void Load(GLuint shader, const char **paths);
	static void Log(GLuint program);
	static void Log(const char *type, GLuint shader);
};

#endif // file

