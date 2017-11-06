/** \file
 * High level interface for OpenGL shaders using managed resources framework.
 */
#ifndef Shader_hpp
#define Shader_hpp

#include "Source.hpp"
#include <memory>
#include <vector>
#include <string>

class Shader : public ManagedSource<Shader>
{
public:

	~Shader();
	bool LoadString(std::string const &code);
	bool LoadFile(std::string const &path);
	bool Link();
	bool Use();

	class SourceCode : public ManagedSource<SourceCode>
	{
	public:

		bool Attach(unsigned id);
		bool Detach(unsigned id);

		static Resources &Manager();
	};

	static Resources &Manager();

protected:

	bool UpdateSource() override;

private:

	std::vector<std::shared_ptr<SourceCode>> shaderSources;
};

#endif // file
