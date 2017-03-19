#ifndef Shader_hpp
#define Shader_hpp

struct Shader
{
	bool LoadFragment(const char **path);
	bool LoadVertex(const char **path);

	bool LoadFragment(const char *path);
	bool LoadVertex(const char *path);

	bool LinkProgram();
	bool UseProgram();

	~Shader();

 protected:

	unsigned program, vertex, fragment;

 private:

	static char *ReadSource(SDL_RWops *ops, size_t &size);
	static char *ReadSource(const char *path, size_t &size);
	static void LoadShader(unsigned shader, const char **paths);
	static void LogShader(const char *type, unsigned shader);
	static void LogProgram(unsigned program);
};

#endif // file

