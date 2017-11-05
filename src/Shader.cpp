#include "Shader.hpp"
#include "Container.hpp"
#include "OpenGL.hpp"
#include "Error.hpp"
#include "SDL.hpp"

#include "stl/algorithm.hpp"

namespace
{
	// Get the shader info log and set it as the SDL error string.
	bool SetShaderError(GLuint const shader)
	{
		GLint length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		if (OpenGL::CheckError("glGetShaderiv"))
		{
			SDL::perror(CannotQueryShader);
		}
		else if (length > 1)
		{
			std::string info(length);
			glGetShaderInfoLog(shader, length, &length, info.data());
			if (OpenGL::CheckError("glGetShaderInfoLog"))
			{
				SDL::perror(CannotQueryShader);
			}
			else
			{
				return SDL::SetError(info);
			}
		}
		return false;
	}

	// Get the program info log and set it as the SDL error string.
	bool SetProgramError(GLuint const program)
	{
		GLint length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		if (OpenGL::CheckError("glGetProgramiv"))
		{
			SDL::perror(CannotQueryProgram);
		}
		else if (length > 1)
		{
			std::string info(length);
			glGetProgramInfoLog(program, length, &length, info.data());
			if (OpenGL::CheckError("glGetProgramInfoLog"))
			{
				SDL::perror(CannotQueryProgram);
			}
			else
			{
				return SDL::SetError(info);
			}
		}
		return false;
	}

	bool CompileShaderSource(GLuint const shader, std::vector<std::string> const &lines)
	{
		std::vector<GLint> lengths;
		std::vector<GLchar*> strings;
		for (auto const &line : lines)
		{
			lengths.emplace_back(line.length());
			strings.emplace_back(line.data());
		}

		glShaderSource(shader, lines.size(), strings.data(), lengths.data());
		if (OpenGL::CheckError("glShaderSource"))
		{
			SDL::perror(CannotCompileShader);
		}
		else
		{
			glCompileShader(shader);
			if (OpenGL::CheckError("glCompileShader"))
			{
				SDL::perror(CannotCompileShader);
			}
			else
			{
				return true;
			}
		}
		return false;
	}

	// Guess the shader type by inspecting the source code
	GLenum GuessShaderType(std::vector<std::string> const &lines)
	{
		auto const line = str::to_upper(lines.front()); // first line

		if (std::string::npos != line.find("FRAGMENT"))
		{
			return GL_FRAGMENT_SHADER;
		}
		if (std::string::npos != line.find("PIXEL"))
		{
			return GL_FRAGMENT_SHADER;
		}
		if (std::string::npos != line.find("VERTEX"))
		{
			return GL_VERTEX_SHADER;
		}
		if (std::string::npos != line.find("GEOMETRY"))
		{
			return GL_GEOMETRY_SHADER;
		}
		if (std::string::npos != line.find("COMPUTE"))
		{
			return GL_COMPUTE_SHADER;
		}
		if (std::string::npos != line.find("CONTROL"))
		{
			return GL_TESS_CONTROL_SHADER;
		}
		if (std::string::npos != line.find("EVALUATION"))
		{
			return GL_TESS_EVALUATION_SHADER;
		}

		return 0;
	}

	class Shader::Source
			: public ::Source, std::enable_shared_from_this<Shader::Source>
	{
	public:

		bool Attach(GLuint const program);
		bool Detach(GLuint const program);

	protected:

		Source()
		{
			id = Shader::SourceManager().Add(this);
		}
		~Source()
		{
			verify(this == Shader::SourceManager().Remove(id));
		}
	};

	class SourceCode : public Shader::Source
	{
	public:

		static std::shared_ptr<SourceCode> Find(std::string const &sourceCode)
		{
			auto const it = stl::find_if(code.set, [&](SourceCode *that)
			{
				return sourceCode == that->sourceCode;
			});
			if (code.set.end() == it)
			{
				return std::make_shared<SourceCode>(sourceCode);
			}
			else
			{
				return it->shared_from_this();
			}
		}

	private:

		std::string const sourceCode;
		bool UpdateSource() override;

		static Container<SourceCode> code;

		SourceCode(std::string const &source) : sourceCode(source)
		{
			verify(code.Add(this));
		}
		~SourceCode()
		{
			verify(code.Remove(this));
		}

	};

	class SourceFile : public Shader::Source
	{
	public:

		static std::shared_ptr<SourceFile> Find(std::string const &sourceFile)
		{
			auto const it = stl::find_if(file.set, [&](SourceCode *that)
			{
				return sourceFile == that->sourceCode;
			});
			if (file.set.end() == it)
			{
				return std::make_shared<SourceCode>(sourceFile);
			}
			else
			{
				return it->shared_from_this();
			}
		}

	private:

		std::string const sourceFile;
		bool UpdateSource() override;

		static Container<SourceFile> file;

		SourceFile(std::string const &source) : sourceFile(source)
		{
			verify(file.Add(this));
		}
		~SourceFile()
		{
			verify(file.Remove(this));
		}
	};
}

bool SourceCode::UpdateSource()
{
	while (true)
	{
		// Split the code into lines
		std::vector<std::string> lines;
		str::split(lines, sourceCode, "\n");
		// Compile the source code lines into a shader
		GLuint const shader = OpenGL::GetShader(Source::id);
		bool const ok = CompileShaderSource(shader, lines);
		if (not ok)
		{
			if (SDL::ShowError(SDL_MESSAGEBOX_WARNING))
			{
				continue; // retry
			}
		}
		return ok;
	}
}

bool SourceFile::UpdateSource()
{
	while (true)
	{
		// Parse the file into lines
		std::vector<std::string> lines;
		{
			std::string line;
			std::ifstream stream(sourceFile);
			while (std::getline(stream, line))
			{
				lines.emplace_back(line);
			}
		}
		// Compile the source code lines into a shader
		GLuint const shader = OpenGL::GetShader(Source::id);
		bool const ok = CompileShaderSource(shader, lines);
		if (not ok)
		{
			if (SDL::ShowError(SDL_MESSAGEBOX_WARNING))
			{
				continue; // retry
			}
		}
		return ok;
	}
}

bool Shader::Source::Attach(GLuint const program)
{
	GLuint const shader = OpenGL::GetShader(Source::id);
	glAttachShader(program, shader);
	if (OpenGL::CheckError("glAttachShader"))
	{
		SDL::perror(CannotUseShader);
		return false;
	}
	return true;
}

bool Shader::Source::Detach(GLuint const program)
{
	GLuint const shader = OpenGL::GetShader(Source::id);
	glDetachShader(program, shader);
	if (OpenGL::CheckError("glDetachShader"))
	{
		SDL::perror(CannotUseShader);
		return false;
	}
	return true;
}

bool Shader::Link()
{
	GLuint const program = OpenGL::GetProgram(Source::id);
	glLinkProgram(program);
	if (OpenGL::CheckError("glLinkProgram"))
	{
		if (SetProgramError(program))
		{
			SDL::ShowError(SDL_MESSAGEBOX_WARNING);
		}
		return false;
	}
	return true;
}

bool Shader::Use()
{
	GLuint const program = OpenGL::GetProgram(Source::id);
	glUseProgram(program);
	if (OpenGL::CheckError("glUseProgram"))
	{
		SDL::perror(CannotUseProgram);
		return false;
	}
	return true;
}

bool Shader::Include(std::string const &code)
{
	auto source = SourceCode::Find(code);
	if (source)
	{
		shaderSources.emplace_back(source);
		return true;
	}
	return false;
}

bool Shader::Import(std::string const &file)
{
	auto source = SourceFile::Find(file);
	if (source)
	{
		shaderSources.emplace_back(source);
		return true;
	}
	return false;
}

bool Shader::UpdateSource()
{
	unsigned count = 0;
	GLuint const program = OpenGL::GetProgram(Source::id);
	for (auto const &shader : shaderSources)
	{
		if (shader->Attach(program))
		{
			++count;
		}
	}
	return shaderSources.size() == count;
}

Shader::~Shader()
{
	unsigned count = 0;
	GLuint const program = OpenGL::GetProgram(Source::id);
	for (auto const &shader : shaderSources)
	{
		if (shader->Detach(program))
		{
			++count;
		}
	}
	assert(shaderSources.size() == count);
	verify(this == ProgramManager().Remove(Source::id));
}

Shader::Shader()
{
	Source::id = ProgramManager().Add(this);
}
