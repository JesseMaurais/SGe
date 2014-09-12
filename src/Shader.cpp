#include "SDL.hpp"
#include "Lua.hpp"
#include "Shader.hpp"
#include "OpenGL.hpp"
#include <cassert>
#include <cstdio>
#include <vector>

void Shader::LoadFragment(const char **paths)
{
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	Load(fragment, paths);
	
	glCompileShader(fragment);
	Log(__func__, fragment);
}

void Shader::LoadFragment(const char *path)
{
	const char *paths[] = {path, NULL};
	LoadFragment(paths);
}

void Shader::LoadFragmentSource(Strings list)
{
	const int size = list.size();
	const char *paths[size + 1];
	for (int item = 0; item < size; ++item)
	{
	 paths[item] = list[item].c_str();
	}
	paths[size] = NULL;
	LoadFragment(paths);
}

void Shader::LoadVertex(const char **paths)
{
	vertex = glCreateShader(GL_VERTEX_SHADER);
	Load(vertex, paths);
	
	glCompileShader(vertex);
	Log(__func__, vertex);
}

void Shader::LoadVertex(const char *path)
{
	const char *paths[] = {path, NULL};
	LoadVertex(paths);
}

void Shader::LoadVertexSource(Strings list)
{
	const int size = list.size();
	const char *paths[size + 1];
	for (int item = 0; item < size; ++item)
	{
	 paths[item] = list[item].c_str();
	}
	paths[size] = NULL;
	LoadVertex(paths);
}

void Shader::LinkProgram(void)
{
	program = glCreateProgram();
	if (glIsShader(fragment))
	{
	 glAttachShader(program, fragment);
	}
	if (glIsShader(vertex))
	{
	 glAttachShader(program, vertex);
	}
	glLinkProgram(program);
	Log(program);
}

void Shader::UseProgram()
{
	glUseProgram(program);
	Log(program);
}

Shader::~Shader()
{
	if (glIsProgram(program))
	{
	 if (glIsShader(fragment))
	 {
	  glDetachShader(program, fragment);
	 }
	 if (glIsShader(vertex))
	 {
	  glDetachShader(program, vertex);
	 }
	 glDeleteProgram(program);
	}

	if (glIsShader(fragment))
	{
	 glDeleteShader(fragment);
	}
	if (glIsShader(vertex))
	{
	 glDeleteShader(vertex);
	}
}

char *Shader::Read(const char *path, int &size)
{
	FILE *file = fopen(path, "r");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		size = ftell(file);
		fseek(file, 0, SEEK_SET);
		
		char *data = new char [size];
		fread(data, size, 1, file);
		
		fclose(file);
		return data;
	}
	else perror(path);
	return 0;
}

void Shader::Load(GLuint shader, const char **paths)
{
	int n = 0; while (paths[n]) ++n;
	
	int *size = new int [n];
	
	const char **data = new const char *[n];
	
	for (int i = 0; i < n; ++i)
	{
	 data[i] = Read(paths[i], size[i]);
	}
	
	glShaderSource(shader, n, data, size);
	
	for (int i = 0; i < n; ++i) delete data[i];
	
	delete data;
	delete size;
}

void Shader::Log(GLuint program)
{
	int n;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &n);
	if (n > 1)
	{
		char *data = new char [n];
		glGetProgramInfoLog(program, n, &n, data);
		SDL_Log("%s", data);
		delete data;
	}
}

void Shader::Log(const char *type, GLuint shader)
{
	int n;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &n);
	if (n > 1)
	{
		char *data = new char [n];
		glGetShaderInfoLog(shader, n, &n, data);
		SDL_Log("%s\n%s\n", type, data);
		delete data;
	}
}

