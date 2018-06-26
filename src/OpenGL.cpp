#include "OpenGL.hpp"
#include "Manager.hpp"
#include "Error.hpp"
#include "Event.hpp"
#include "SDL.hpp"

// Resource management implementation

namespace
{
	// User event codes to queue updates
	enum NotifyEventCode : Sint32
	{
		NotifyTextures,
		NotifyBuffers,
		NotifyPrograms,
		NotifyVertexes,
		NotifyFragments,
		NotifyGeometries,
		NotifyComputers,
	};

	using ManagerType = Manager<Slot<GLuint>*, GLuint>;

	// Collect resource managers so we do not miss any
	class ManagerSet : public ManagerType
	{
	public:

		static void Initialize()
		{
			for (auto const &pair : set)
			{
				pair.second->Initialize();
			}
		}

		static void Release()
		{
			for (auto const &pair : set)
			{
				pair.second->Release();
			}
		}

		static void Update(NotifyEventCode const code)
		{
			set.at(code)->Update();
		}

	protected:

		ManagerSet(NotifyEventCode const code)
		{
			set.insert_or_assign(code, this);
		}

		~ManagerSet()
		{
			for (auto it = set.begin(); it != set.end(); ++it)
			{
				if (this == it->second)
				{
					set.erase(it);
					break;
				}
			}
		}

	private:

		static std::map<NotifyEventCode, ManagerType*> set;
	};

	std::map<NotifyEventCode, ManagerType*> ManagerSet::set;

	// Generic manager for common notifications
	template <NotifyEventCode NotifyCode>
	class UpdateManager : public ManagerSet
	{
	protected:

		UpdateManager()
		: ManagerSet(NotifyCode)
		{}

	private:

		bool SendUpdate() const override
		{
			return SDL::SendUserEvent(UpdateOpenGL, NotifyCode);
		}
	};

	// Specific resource managers

	class TextureManager final : public UpdateManager<NotifyTextures>
	{
	private:

		TextureManager() = default;

	public:

		static TextureManager &Instance()
		{
			static TextureManager singleton;
			return singleton;
		}

		void Generate(std::vector<GLuint> &ids) const override
		{
			glGenTextures(ids.size(), ids.data());
			if (OpenGL::CheckError("glGenTextures"))
			{
				SDL::LogError(CannotAllocateResource);
			}
		}

		void Destroy(std::vector<GLuint> const &ids) const override
		{
			glDeleteTextures(ids.size(), ids.data());
			if (OpenGL::CheckError("glDeleteTextures"))
			{
				SDL::LogError(CannotFreeResource);
			}
		}
	};

	class BufferManager final : public UpdateManager<NotifyBuffers>
	{
	private:

		BufferManager() = default;

	public:

		static BufferManager &Instance()
		{
			static BufferManager singleton;
			return singleton;
		}

		void Generate(std::vector<GLuint> &ids) const override
		{
			glGenBuffers(ids.size(), ids.data());
			if (OpenGL::CheckError("glGenBuffers"))
			{
				SDL::LogError(CannotAllocateResource);
			}
		}

		void Destroy(std::vector<GLuint> const &ids) const override
		{
			glDeleteBuffers(ids.size(), ids.data());
			if (OpenGL::CheckError("glDeleteTextures"))
			{
				SDL::LogError(CannotFreeResource);
			}
		}
	};

	class ProgramManager final : public UpdateManager<NotifyPrograms>
	{
	private:

		ProgramManager() = default;

	public:

		static ProgramManager &Instance()
		{
			static ProgramManager singleton;
			return singleton;
		}

		void Generate(std::vector<GLuint> &ids) const override
		{
			for (GLuint &program : ids)
			{
				program = glCreateProgram();
				if (OpenGL::CheckError("glCreateProgram"))
				{
					SDL::LogError(CannotAllocateResource);
				}
			}
		}

		void Destroy(std::vector<GLuint> const &ids) const override
		{
			for (GLuint const program : ids)
			{
				glDeleteProgram(program);
				if (OpenGL::CheckError("glDeleteProgram"))
				{
					SDL::LogError(CannotFreeResource);
				}
			}
		}
	};

	template <NotifyEventCode NotifyCode, GLenum ShaderType>
	class ShaderManager : public UpdateManager<NotifyCode>
	{
	private:

		ShaderManager() = default;

	public:

		static ShaderManager &Instance()
		{
			static ShaderManager singleton;
			return singleton;
		}

		void Generate(std::vector<GLuint> &ids) const override
		{
			for (GLuint &shader : ids)
			{
				shader = glCreateShader(ShaderType);
				if (OpenGL::CheckError("glCreateShader"))
				{
					SDL::LogError(CannotAllocateResource);
				}
			}
		}

		void Destroy(std::vector<GLuint> const &ids) const override
		{
			for (GLuint const shader : ids)
			{
				glDeleteShader(shader);
				if (OpenGL::CheckError("glDeleteShader"))
				{
					SDL::LogError(CannotFreeResource);
				}
			}
		}
	};

	// Map a shader type to a shader resource manager
	ManagerType &FindShaderManager(GLenum const type)
	{
		using VertexManager = ShaderManager<NotifyVertexes, GL_VERTEX_SHADER>;
		using FragmentManager = ShaderManager<NotifyFragments, GL_FRAGMENT_SHADER>;
		using GeometryManager = ShaderManager<NotifyGeometries, GL_GEOMETRY_SHADER>;
		using ComputeManager = ShaderManager<NotifyComputers, GL_COMPUTE_SHADER>;

		switch (type)
		{
		default:
		case GL_VERTEX_SHADER:
			return VertexManager::Instance();
		case GL_FRAGMENT_SHADER:
			return FragmentManager::Instance();
		case GL_GEOMETRY_SHADER:
			return GeometryManager::Instance();
		case GL_COMPUTE_SHADER:
			return ComputeManager::Instance();
		}
	}

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

// Bind observers to the correct manager

OpenGL::Texture::Texture(Observer observer)
: Slot(TextureManager::Instance(), observer)
{}

OpenGL::Buffer::Buffer(Observer observer)
: Slot(BufferManager::Instance(), observer)
{}

OpenGL::Program::Program(Observer observer)
: Slot(ProgramManager::Instance(), observer)
{}

OpenGL::Shader::Shader(GLenum type, Observer observer)
: Slot(FindShaderManager(type), observer)
{}

// Error utility functions

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

// Context singleton

SDL_GLContext OpenGL::GetContext(SDL_Window *window)
{
	static class Context
	{
	private:

		static void Initialize()
		{
			ManagerSet::Initialize();
		}

		static void Release()
		{
			ManagerSet::Release();
		}

		static void UpdateHandler(SDL_Event const &event)
		{
			ManagerSet::Update(static_cast<NotifyEventCode>(event.user.code));
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
						GLenum const error = glewInit();
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
					// Init resources
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
				// Free resources
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
