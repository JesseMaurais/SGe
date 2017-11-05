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
	bool LoadString(std::string const &code);
	bool LoadFile(std::string const &path);
	bool Link();
	bool Use();

protected:

	static Resources &SourceManager();
	static Resources &ProgramManager();

private:

	class SourceCode;
	std::vector<std::shared_ptr<SourceCode>> shaderSources;
	bool UpdateSource() override;
};

#endif // file
