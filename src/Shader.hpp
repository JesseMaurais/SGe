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
	bool Link() const;
	bool Use() const;

	class Source
	{
	public:

		bool Attach(GLuint program) const;
		bool Detach(GLuint program) const;

	protected:

		void SetShaderType(GLenum type);
		virtual bool Update(GLuint shader) = 0;
		OpenGL::Shader slot;
		GLuint shader;
		GLenum type;
	};

protected:

	virtual void Update(GLuint program) = 0;
	OpenGL::Program slot;
	GLuint program;

private:

	std::vector<std::shared_ptr<Source>> shaderSources;
};

#endif // file
