#ifndef Shader_hpp
#define Shader_hpp

#include "Source.hpp"
#include <memory>
#include <vector>
#include <string>

class Shader : public Source
{
public:

	Shader();
	~Shader();
	bool Include(std::string const &sourceCode);
	bool Import(std::string const &sourcePath);
	bool Link();
	bool Use();

protected:

	static Resources &SourceManager();
	static Resources &ProgramManager();

private:

	class Source;
	std::vector<std::shared_ptr<Source>> shaderSources;
	bool UpdateSource() override;
};

#endif // file
