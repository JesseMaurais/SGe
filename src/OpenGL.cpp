#include "OpenGL.hpp"
#include "Shader.hpp"
#include "Manager.hpp"
#include "Video.hpp"
#include "Error.hpp"
#include "Event.hpp"
#include "SDL.hpp"

#include "stl/algorithm.hpp"

// Resource management implementation

namespace
{
	// User event codes to queue updates with
	enum UpdateEventCode : Uint32
	{
		UpdateTextures,
		UpdateBuffers,
		UpdateShaders,
		UpdatePrograms,
	};

	// Generic resource manager for any GLuint based id type
	template <UpdateEventCode UpdateCode, typename Slot>
	class UpdateManager : public Manager<Slot*, GLuint>
	{
	private:

		bool SendUpdate() override
		{
			return SDL::SendUserEvent(UpdateOpenGL, UpdateCode);
		}
	};


	class TextureManager final : public UpdateManager<UpdateTextures>
	{
	private:

		TextureManager() = default;

	public:

		static TextureManager &Instance()
		{
			static TextureManager singleton;
			return singleton;
		}

		void Generate(std::vector<GLuint> &ids) override
		{
			glGenTextures(ids.size(), ids.data());
			if (OpenGL::CheckError("glGenTextures"))
			{
				SDL::LogError(CannotAllocateResource);
			}
		}

		void Destroy(std::vector<GLuint> const &ids) override
		{
			glDeleteTextures(ids.size(), ids.data());
			if (OpenGL::CheckError("glDeleteTextures"))
			{
				SDL::LogError(CannotFreeResource);
			}
		}
	};

	class BufferManager final : public UpdateManager<UpdateBuffers>
	{
	private:

		BufferManager() = default;

	public:

		static BufferManager &Instance()
		{
			static BufferManager singleton;
			return singleton;
		}

		void Generate(std::vector<GLuint> &ids) override
		{
			glGenBuffers(ids.size(), ids.data());
			if (OpenGL::CheckError("glGenBuffers"))
			{
				SDL::LogError(CannotAllocateResource);
			}
		}

		void Destroy(std::vector<GLuint> const &ids) override
		{
			glDeleteBuffers(ids.size(), ids.data());
			if (OpenGL::CheckError("glDeleteTextures"))
			{
				SDL::LogError(CannotFreeResource);
			}
		}
	};

	class ProgramManager final : public UpdateManager<UpdatePrograms>
	{
	private:

		ProgramManager() = default;

	public:

		static ProgramManager &Instance()
		{
			static ProgramManager singleton;
			return singleton;
		}

		void Generate(std::vector<GLuint> &ids) override
		{
			stl::generate(ids, []()
			{
				GLuint const program = glCreateProgram();
				if (OpenGL::CheckError("glCreateProgram"))
				{
					SDL::LogError(CannotAllocateResource);
				}
				return program;
			});
		}

		void Destroy(std::vector<GLuint> const &ids) override
		{
			stl::for_each(ids, [](GLuint const program)
			{
				glDeleteProgram(program);
				if (OpenGL::CheckError("glDeleteProgram"))
				{
					SDL::LogError(CannotFreeResource);
				}
			});
		}
	};

	struct ShaderPair { GLenum type; GLuint shader; };
	class ShaderManager final : public Manager<ShaderPair>
	{
	private:

		ShaderManager() = default;

		bool SendUpdate() override
		{
			return SDL::SendUserEvent(UpdateOpenAL, UpdateShaders);
		}

	public:

		static ShaderManager &Instance()
		{
			static ShaderManager singleton;
			return singleton;
		}

		void Generate(std::vector<ShaderPair> &pairs) override
		{
			stl::for_each(pairs, [](ShaderPair &pair)
			{
				pair.shader = glCreateShader(pair.type);
				if (OpenGL::CheckError("glCreateShader"))
				{
					SDL::LogError(CannotAllocateResource);
				}
			});
		}

		void Destroy(std::vector<ShaderPair> const &pairs) override
		{
			stl::for_each(pairs, [](ShaderPair const &pair)
			{
				glDeleteShader(pair.shader);
				if (OpenGL::CheckError("glDeleteShader"))
				{
					SDL::LogError(CannotFreeResource);
				}
			});
		}
	};

	// Convert error code to error string.
	char const *ErrorString(GLenum error)
	{
		union
		{
		 const GLubyte *bytes;
		 const char *string;
		};
		bytes = gluErrorString(error);
		return string;
	}
}

// OpenGL error utility functions

bool OpenGL::SetError(const char *origin, GLenum error)
{
	return SDL::SetError(ColonSeparator, origin, ErrorString(error));
}

bool OpenGL::CheckError(const char *origin)
{
	GLenum const error = glGetError();
	return error and OpenGL::SetError(origin, error);
}

bool OpenGL::LogError(const char *origin)
{
	GLenum const error = glGetError();
	return error and SDL::LogError(origin, ErrorString(error));
}

// OpenGL resource utility functions

GLuint OpenGL::GetTexture(unsigned index)
{
	return TextureManager::Instance().Data(index);
}

Resources &VideoTexture::Manager()
{
	return TextureManager::Instance();
}

GLuint OpenGL::GetBuffer(unsigned index)
{
	return BufferManager::Instance().Data(index);
}

Resources &VideoBuffer::Manager()
{
	return BufferManager::Instance();
}

GLuint OpenGL::GetProgram(unsigned index)
{
	return ProgramManager::Instance().Data(index);
}

Resources &Shader::Manager()
{
	return ProgramManager::Instance();
}

Resources &Shader::SourceCode::Manager()
{
	return ShaderManager::Instance();
}

GLuint OpenGL::GetShader(unsigned index)
{
	return ShaderManager::Instance().Data(index).shader;
}

void OpenGL::SetShaderType(unsigned index, GLenum type)
{
	ShaderManager::Instance().Data(index).type = type;
}

void *OpenGL::GetContext(SDL_Window *window)
{
	static class Context
	{
	private:

		static void Initialize()
		{
			// Generate textures
			TextureManager::Instance().Initialize();
			// Generate buffers
			BufferManager::Instance().Initialize();
			// Create shaders
			ShaderManager::Instance().Initialize();
			// Create programs
			ProgramManager::Instance().Initialize();
		}

		static void Release()
		{
			// Delete textures
			TextureManager::Instance().Release();
			// Delete buffers
			BufferManager::Instance().Release();
			// Delete shaders
			ShaderManager::Instance().Release();
			// Delete programs
			ProgramManager::Instance().Release();
		}

		static void UpdateHandler(SDL_Event const &event)
		{
			switch (event.user.code)
			{
			case UpdateTextures:
				TextureManager::Instance().Update();
				break;
			case UpdateBuffers:
				BufferManager::Instance().Update();
				break;
			case UpdateShaders:
				ShaderManager::Instance().Update();
				break;
			case UpdatePrograms:
				ProgramManager::Instance().Update();
				break;
			default:
				assert(not "OpenGL event code");
			}
		}

		ScopedEventHandler updater;

	public:

		SDL_GLContext context = nullptr;

		Context() = default;
		Context(SDL_Window *window)
		: updater(SDL::UserEvent(UpdateOpenGL), UpdateHandler)
		{
			if (window)
			{
				// Create compatible with given window
				context = SDL_GL_CreateContext(window);
				if (context)
				{
					// Attach if creation succeeded
					if (SDL_GL_MakeCurrent(window, context))
					{
						SDL::LogError("SDL_GL_MakeCurrent");
					}
					// Once only
					static bool init = false;
					if (not init)
					{
						// Initialize extensions
						GLenum error = glewInit();
						if (error)
						{
							union {
								const GLubyte *bytes;
								const char *string;
							};
							// Log without setting error string
							bytes = glewGetErrorString(error);
							SDL::LogError("glewInit", string);
						}
						else init = true;
					}
					Initialize();
				}
				else
				{
					// Set error string and return null
					SDL::LogError("SDL_GL_CreateContext");
				}
			}
		}
		~Context()
		{
			// Free if it was created
			if (context)
			{
				Release();
				// Detach before deletion
				if (SDL_GL_GetCurrentContext() == context)
				{
					if (SDL_GL_MakeCurrent(nullptr, nullptr))
					{
						SDL::LogError("SDL_GL_MakeCurrent");
					}
				}
				SDL_GL_DeleteContext(context);
			}
		}

	} singleton;
	// Either new or not initialized
	if (window or not singleton.context)
	{
		// Update for compatibility with given window
		singleton = Context(window);
	}
	return singleton.context;
}
