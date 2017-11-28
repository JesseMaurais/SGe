#include "Shader.hpp"
#include "Collection.hpp"
#include "OpenGL.hpp"
#include "Error.hpp"
#include "SDL.hpp"
#include "std.hpp"

namespace
{
	// Get the shader info log and set it as the SDL error string
	bool SetShaderError(GLuint const shader)
	{
		GLint length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		if (OpenGL::CheckError("glGetShaderiv"))
		{
			SDL::LogError(CannotQueryShader);
		}
		else if (length > 1)
		{
			std::vector<char> info(length);
			glGetShaderInfoLog(shader, info.size(), &length, info.data());
			if (OpenGL::CheckError("glGetShaderInfoLog"))
			{
				SDL::LogError(CannotQueryShader);
			}
			else
			{
				return SDL::SetError(info.data());
			}
		}
		return false;
	}

	// Get the program info log and set it as the SDL error string
	bool SetProgramError(GLuint const program)
	{
		GLint length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		if (OpenGL::CheckError("glGetProgramiv"))
		{
			SDL::LogError(CannotQueryProgram);
		}
		else if (length > 1)
		{
			std::vector<char> info(length);
			glGetProgramInfoLog(program, info.size(), &length, info.data());
			if (OpenGL::CheckError("glGetProgramInfoLog"))
			{
				SDL::LogError(CannotQueryProgram);
			}
			else
			{
				return SDL::SetError(info.data());
			}
		}
		return false;
	}

	// Set the shader source code, cut into \n delimited lines, and compile it
	bool CompileShaderSource(GLuint const shader, std::vector<std::string> const &lines)
	{
		// Collect raw C strings
		std::vector<GLint> lengths;
		std::vector<GLchar const *> strings;
		for (auto const &line : lines)
		{
			lengths.emplace_back(line.size());
			strings.emplace_back(line.data());
		}
		// Send the raw C strings to the OpenGL object and compile it
		glShaderSource(shader, lines.size(), strings.data(), lengths.data());
		if (OpenGL::CheckError("glShaderSource"))
		{
			SDL::LogError(CannotCompileShader);
		}
		else
		{
			glCompileShader(shader);
			if (OpenGL::CheckError("glCompileShader"))
			{
				SDL::LogError(CannotCompileShader);
			}
			else
			{
				return true;
			}
		}
		return false;
	}

	// Guess the shader type by inspecting the source code
	GLenum GuessShaderType(std::string const &sourceCode)
	{
		// Inspect the first line (usually a comment) for a keyword
		auto line = sourceCode.substr(0, sourceCode.find("\n"));
		line = stl::to_upper(line); // case insensitive match

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

	// Shader source code loaded from a string.

	class SourceString : public Shader::SourceCode
	, public std::enable_shared_from_this<SourceString>
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
				return std::make_shared<SourceString>(sourceString);
			}
			else
			{
				return (*it)->shared_from_this();
			}
		}

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

	private:

		std::string const sourceString;
		bool UpdateSource() override;

		static Collection<SourceString> code;
	};

	Collection<SourceString> SourceString::code;

	// Shader source code loaded from a file.

	class SourceFile : public Shader::SourceCode
	, public std::enable_shared_from_this<SourceFile>
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
				return std::make_shared<SourceFile>(sourceFile);
			}
			else
			{
				return (*it)->shared_from_this();
			}
		}

		SourceFile(std::string const &source) : sourceFile(source)
		{
			verify(code.Add(this));
		}
		~SourceFile()
		{
			verify(code.Remove(this));
		}

	private:

		std::string const sourceFile;
		bool UpdateSource() override;

		static Collection<SourceFile> code;
	};

	Collection<SourceFile> SourceFile::code;
}

bool SourceString::UpdateSource()
{
	while (true)
	{
		// Split the code into lines.
		std::vector<std::string> lines;
		stl::split(lines, sourceString, std::string("\n"));
		// Compile the source code lines into a shader.
		GLuint const shader = OpenGL::GetShader(Source::id);
		bool const ok = CompileShaderSource(shader, lines);
		if (not ok and SetShaderError(shader))
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
		if (not ok and SetShaderError(shader))
		{
			if (SDL::ShowError(SDL_MESSAGEBOX_WARNING))
			{
				continue; // retry
			}
		}
		return ok;
	}
}

bool Shader::SourceCode::Attach(unsigned id)
{
	GLuint const shader = OpenGL::GetShader(Source::id);
	GLuint const program = OpenGL::GetProgram(id);
	glAttachShader(program, shader);
	if (OpenGL::CheckError("glAttachShader"))
	{
		SDL::LogError(CannotUseShader);
		return false;
	}
	return true;
}

bool Shader::SourceCode::Detach(unsigned id)
{
	GLuint const shader = OpenGL::GetShader(Source::id);
	GLuint const program = OpenGL::GetProgram(id);
	glDetachShader(program, shader);
	if (OpenGL::CheckError("glDetachShader"))
	{
		SDL::LogError(CannotUseShader);
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
				SDL::LogError(CannotLinkProgram);
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
		SDL::LogError(CannotUseProgram);
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
	for (auto const &shader : shaderSources)
	{
		if (shader->Attach(Source::id))
		{
			++count;
		}
	}
	return shaderSources.size() == count;
}

Shader::~Shader()
{
	unsigned count = 0;
	for (auto const &shader : shaderSources)
	{
		if (shader->Detach(Source::id))
		{
			++count;
		}
	}
	assert(shaderSources.size() == count);
}

