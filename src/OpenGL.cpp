#include "OpenGL.hpp"
#include "Shader.hpp"
#include "Manager.hpp"
#include "Error.hpp"
#include "Event.hpp"
#include "SDL.hpp"

#include "stl/algorithm.hpp"

namespace
{
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

	enum UpdateEventCode : Uint32
	{
		UpdateTextures,
		UpdateShaders,
		UpdatePrograms,
	};

	template <UpdateEventCode UpdateCode>
	class UpdateManager : public Manager<GLuint>
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
				SDL::perror(CannotCreateResource);
			}
		}

		void Destroy(std::vector<GLuint> const &ids) override
		{
			glDeleteTextures(ids.size(), ids.data());
			if (OpenGL::CheckError("glDeleteTextures"))
			{
				SDL::perror(CannotDeleteResource);
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
				auto const id = glCreateProgram();
				if (OpenGL::CheckError("glCreateProgram"))
				{
					SDL::perror(CannotCreateResource);
				}
				return id;
			});
		}

		void Destroy(std::vector<GLuint> const &ids) override
		{
			stl::for_each(ids, [](GLuint id)
			{
				glDeleteProgram(id);
				if (OpenGL::CheckError("glDeleteProgram"))
				{
					SDL::perror(CannotDeleteResource);
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
					SDL::perror(CannotCreateResource);
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
					SDL::perror(CannotDeleteResource);
				}
			});
		}
	};
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
	return error and SDL::perror(origin, ErrorString(error));
}

GLuint OpenGL::GetTexture(unsigned index)
{
	return TextureManager::Instance().Data(index);
}

Resources &Shader::ProgramManager()
{
	return ProgramManager::Instance();
}

GLuint OpenGL::GetProgram(unsigned index)
{
	return ProgramManager::Instance().Data(index);
}

Resources &Shader::SourceManager()
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
	static struct Context
	{
		SDL_GLContext context = nullptr;

		Context() = default;
		Context(SDL_Window *window)
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
						SDL::perror("SDL_GL_MakeCurrent");
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
							SDL::perror("glewInit", string);
						}
						else init = true;
					}
					// Generate textures
					TextureManager::Instance().Initialize();
					// Create shaders
					ShaderManager::Instance().Initialize();
					// Create programs
					ProgramManager::Instance().Initialize();
					// Register update handler
					SDL::PushEventHandler(UpdateOpenGL, UpdateHandler);
				}
				else
				{
					// Set error string and return null
					SDL::perror("SDL_GL_CreateContext");
				}
			}
		}
		~Context()
		{
			// Free if it was created
			if (context)
			{
				// Unregister event handler
				SDL::PopEventHandler(UpdateOpenGL);
				// Delete textures
				TextureManager::Instance().Release();
				// Delete shaders
				ShaderManager::Instance().Release();
				// Delete programs
				ProgramManager::Instance().Release();
				// Detach before deletion
				if (SDL_GL_GetCurrentContext() == context)
				{
					if (SDL_GL_MakeCurrent(nullptr, nullptr))
					{
						SDL::perror("SDL_GL_MakeCurrent");
					}
				}
				SDL_GL_DeleteContext(context);
			}
		}

	private:

		static bool UpdateHandler(SDL_Event const &event)
		{
			assert(UserEventType::UpdateOpenGL == event.user.type);
			switch (event.user.code)
			{
			case UpdateEventCode::UpdateTextures:
				TextureManager::Instance().Update();
				break;
			case UpdateEventCode::UpdateShaders:
				ShaderManager::Instance().Update();
				break;
			case UpdateEventCode::UpdatePrograms:
				ProgramManager::Instance().Update();
				break;
			default:
				assert(not "OpenGL event code");
				return false;
			}
			return true;
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
