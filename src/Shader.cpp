#include "Shader.hpp"
#include "Strings.hpp"
#include "OpenGL.hpp"
#include "Error.hpp"
#include "SDL.hpp"


bool Shader::LoadFragment(const char **paths)
{
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	if (fragment)
	{
		LoadShader(fragment, paths);
		glCompileShader(fragment);
		LogShader(__func__, fragment);
		return true;
	}
	else
	{
		auto error = glGetError();
		OpenGL_SetError("glCreateShader", error);
		return false;
	}
}

void Shader::LoadFragment(const char *path)
{
	const char *paths[] = {path, NULL};
	LoadFragment(paths);
}

void Shader::LoadVertex(const char **paths)
{
	vertex = glCreateShader(GL_VERTEX_SHADER);
	LoadShader(vertex, paths);
	
	glCompileShader(vertex);
	LogShader(__func__, vertex);
}

void Shader::LoadVertex(const char *path)
{
	const char *paths[] = {path, NULL};
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
	LogProgram(program);
}

void Shader::UseProgram()
{
	glUseProgram(program);
	LogProgram(program);
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

char *Shader::ReadSource(const char *path, size_t &size)
{
	SDL_RWops *ops = SDL_RWFromFile(path, "r");
	if (ops)
	{
		SDL_RWseek(ops, 0, RW_SEEK_END);
		size = SDL_RWtell(ops);
		SDL_RWseek(ops, 0, RW_SEEK_SET);
		
		char *data = new char [size];
		if (!SDL_RWread(ops, data, size, sizeof(char)))
		{
			SDL_perror(path);
		}
		
		if (SDL_RWclose(ops))
		{
			SDL_perror(path);
		}
		return data;
	}
	SDL_perror(path);
	return nullptr;
}

void Shader::LoadShader(GLuint shader, const char **paths)
{
	size_t n = 0;
	while (paths[n++]);
	size_t *size = new size_t [n];
	
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

void Shader::LogShader(const char *type, GLuint shader)
{
	int n;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &n);
	if (OpenGL_SetError("GL_INFO_LOG_LENGTH"))
	{
		ShowError(CannotUseShader);
	}
	else
	if (n > 1)
	{
		char *data = new char [n];
		glGetShaderInfoLog(shader, n, &n, data);
		if (OpenGL_SetError("glGetShaderInfoLog"))
		{
			ShowError(CannotUseShader);
		}
		else
		{
			SDL_Log("%s\n%s\n", type, data);
		}
		delete data;
	}
}

void Shader::LogProgram(unsigned program)
{
	int n;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &n);
	if (OpenGL_SetError("GL_INFO_LOG_LENGTH"))
	{
		ShowError(CannotUseShaderProgram);
	}
	else
	if (n > 1)
	{
		char *data = new char [n];
		glGetProgramInfoLog(program, n, &n, data);
		if (OpenGL_SetError("glGetProgramInfoLog"))
		{
			ShowError(CannotUseShaderProgram);
		}
		else
		{
			SDL_Log("%s", data);
		}
		delete data;
	}
}
