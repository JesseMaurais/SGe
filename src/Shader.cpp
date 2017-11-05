#include "Shader.hpp"
#include "Collection.hpp"
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

	// Set the shader source code, cut into \n delimited lines, and compile it.
	bool CompileShaderSource(GLuint const shader, std::vector<std::string> const &lines)
	{
		// Collect raw C strings.
		std::vector<GLint> lengths;
		std::vector<GLchar*> strings;
		for (auto const &line : lines)
		{
			lengths.emplace_back(line.size());
			strings.emplace_back(line.data());
		}
		// Send the raw C strings to the OpenGL object and compile it.
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

	// Guess the shader type by inspecting the source code.
	GLenum GuessShaderType(std::string const &sourceCode)
	{
		// Inspect the first line (usually a comment) for a keyword.
		auto const line = sourceCode.substr(0, sourceCode.find("\n"));
		line = str::to_upper(line); // case insensitive match

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

	class Shader::SourceCode : public Source, std::enable_shared_from_this<SourceCode>
	{
	public:

		bool Attach(GLuint const program);
		bool Detach(GLuint const program);

	protected:

		SourceCode()
		{
			id = Shader::SourceManager().Add(this);
		}
		~SourceCode()
		{
			verify(this == Shader::SourceManager().Remove(id));
		}
	};

	class SourceString : public Shader::SourceCode
	{
	public:

		static std::shared_ptr<SourceCode> Find(std::string const &sourceString)
		{
			auto const it = stl::find_if(code.set, [&](SourceString *that)
			{
				return sourceString == that->sourceString;
			});
			// Share or make new object.
			if (code.set.end() == it)
			{
				return std::make_shared<SourceCode>(sourceString);
			}
			else
			{
				return it->shared_from_this();
			}
		}

	private:

		std::string const sourceString;
		bool UpdateSource() override;

		static Collection<SourceString> code;

		SourceString(std::string const &source) : sourceString(source)
		{
			GLenum const type = GuessShaderType(source);
			OpenGL::SetShaderType(Source::id, type);
			verify(code.Add(this));
		}
		~SourceString()
		{
			verify(code.Remove(this));
		}

	};

	class SourceFile : public Shader::SourceCode
	{
	public:

		static std::shared_ptr<SourceCode> Find(std::string const &sourceFile)
		{
			auto const it = stl::find_if(code.set, [&](SourceFile *that)
			{
				return sourceFile == that->sourceFile;
			});
			// Share or make new object.
			if (code.set.end() == it)
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

		static Collection<SourceFile> code;

		SourceFile(std::string const &source) : sourceFile(source)
		{
			verify(code.Add(this));
		}
		~SourceFile()
		{
			verify(code.Remove(this));
		}
	};
}

bool SourceString::UpdateSource()
{
	while (true)
	{
		// Split the code into lines.
		std::vector<std::string> lines;
		str::split(lines, sourceString, "\n");
		// Compile the source code lines into a shader.
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
		// Parse the file into lines.
		std::vector<std::string> lines;
		{
			std::ifstream stream(sourceFile);
			std::string line;
			while (std::getline(stream, line))
			{
				lines.emplace_back(line);
			}
		}
		// Compile the source code lines into a shader.
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

bool Shader::SourceCode::Attach(GLuint const program)
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

bool Shader::SourceCode::Detach(GLuint const program)
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
	while (true)
	{
		GLuint const program = OpenGL::GetProgram(Source::id);
		glLinkProgram(program);
		if (OpenGL::CheckError("glLinkProgram"))
		{
			if (SetProgramError(program))
			{
				if (SDL::ShowError(SDL_MESSAGEBOX_WARNING))
				{
					continue; // retry
				}
				SDL::perror(CannotLinkProgram);
			}
			return false;
		}
		return true;
	}
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

bool Shader::LoadString(std::string const &code)
{
	auto source = SourceString::Find(code);
	if (source)
	{
		shaderSources.emplace_back(source);
		return true;
	}
	return false;
}

bool Shader::LoadFile(std::string const &file)
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
