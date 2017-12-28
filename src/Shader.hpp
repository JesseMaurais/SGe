/** \file
 * High level interface for OpenGL shaders using managed resources framework.
 */
#ifndef Shader_hpp
#define Shader_hpp

#include "OpenGL.hpp"
#include <memory>
#include <vector>
#include <string>

class Shader
{
public:

	bool LoadString(std::string const &code);
	bool LoadFile(std::string const &path);
	bool Link();
	bool Use();

	class Source
	{
	public:

		Source(GLenum type);
		virtual bool UpdateSource(GLuint shader) = 0;
		bool Attach(GLuint program);
		bool Detach(GLuint program);

	protected:

		OpenGL::Shader slot;
		GLuint shader;
	};

protected:

	OpenGL::Program slot;
	GLuint program;

private:

	std::vector<std::shared_ptr<Source>> shaderSources;
};

#endif // file
