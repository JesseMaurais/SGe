#include "Shader.hpp"
#include "Error.hpp"
#include "SDL.hpp"
#include "std.hpp"

namespace
{
	// Get the shader info log and set it as the SDL error string
	bool SetShaderError(GLuint shader)
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
	bool SetProgramError(GLuint program)
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
	GLenum GuessShaderType(std::string const &code)
	{
		// Inspect the first line (usually a comment) for a keyword
		auto line = code.substr(0, code.find("\n"));
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

	class SourceCode : public Shader::Source
	, public std::enable_shared_from_this<SourceCode>
	{
	public:

		static std::shared_ptr<Shader::Source> Find(std::string const &code)
		{
			auto const it = stl::find_if(set, [&code](SourceCode *that)
			{
				return code == that->code;
			});
			// Share or make new object.
			if (set.end() == it)
			{
				return std::make_shared<SourceCode>(code);
			}
			else
			{
				return (*it)->shared_from_this();
			}
		}

		bool Update(GLuint shader) override
		{
			Source::shader = shader;

			while (true)
			{
				// Split the code into lines.
				std::vector<std::string> lines;
				stl::split(lines, code, std::string("\n"));
				// Compile the source code lines into a shader.
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

		SourceCode(std::string const &string) : code(string)
		{
			SetShaderType(GuessShaderType(code));
			set.insert(this);
		}

		~SourceCode()
		{
			set.erase(this);
		}

	private:

		std::string const code;
		static std::set<SourceCode*> set;
	};

	std::set<SourceCode*> SourceCode::set;

	// Shader source code loaded from a file.

	class SourceFile : public Shader::Source
	, public std::enable_shared_from_this<SourceFile>
	{
	public:

		static std::shared_ptr<Shader::Source> Find(std::string const &path)
		{
			auto const it = stl::find_if(set, [&](SourceFile *that)
			{
				return path == that->path;
			});
			// Share or make new object.
			if (set.end() == it)
			{
				return std::make_shared<SourceFile>(path);
			}
			else
			{
				return (*it)->shared_from_this();
			}
		}

		bool Update(GLuint shader) override
		{
			Source::shader = shader;

			while (true)
			{
				// Parse the file into lines.
				std::vector<std::string> lines;
				{
					std::ifstream stream(path);
					std::string line;
					while (std::getline(stream, line))
					{
						lines.emplace_back(line);
					}
				}
				// Compile the source code lines into a shader.
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

		SourceFile(std::string const &file) : path(file)
		{
			set.insert(this);
		}

		~SourceFile()
		{
			set.erase(this);
		}

	private:

		std::string const path;

		static std::set<SourceFile*> set;
	};

	std::set<SourceFile*> SourceFile::set;
}

void Shader::Source::SetShaderType(GLenum newType)
{
	if (newType != type)
	{
		type = newType;
		slot = OpenGL::Shader(type, [this](GLuint shader)
		{
		 	Update(shader);
		});
	}
}

bool Shader::Source::Attach(GLuint program) const
{
	if (slot and glIsShader(shader))
	{
		glAttachShader(program, shader);
		if (OpenGL::CheckError("glAttachShader"))
		{
			SDL::LogError(CannotUseShader);
			return false;
		}
	}
	return true;
}

bool Shader::Source::Detach(GLuint program) const
{
	if (slot and glIsShader(shader))
	{
		glDetachShader(program, shader);
		if (OpenGL::CheckError("glDetachShader"))
		{
			SDL::LogError(CannotUseShader);
			return false;
		}
	}
	return true;
}

bool Shader::Link() const
{
	while (true)
	{
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

bool Shader::Use() const
{
	while (true)
	{
		glUseProgram(program);
		if (OpenGL::CheckError("glUseProgram"))
		{
			if (SetProgramError(program))
			{
				if (SDL::ShowError(SDL_MESSAGEBOX_WARNING))
				{
					continue; // retry
				}
				SDL::LogError(CannotUseProgram);
			}
			return false;
		}
		return true;
	}
}

bool Shader::LoadString(std::string const &code)
{
	auto source = SourceCode::Find(code);
	if (source)
	{
		shaderSources.emplace_back(source);
		return true;
	}
	return false;
}

bool Shader::LoadFile(std::string const &path)
{
	auto source = SourceFile::Find(path);
	if (source)
	{
		shaderSources.emplace_back(source);
		return true;
	}
	return false;
}
