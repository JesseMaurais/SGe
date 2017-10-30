#include "OpenGL.hpp"
#include "Strings.hpp"
#include "Manager.hpp"
#include "SDL.hpp"

namespace
{
	class TextureManager final : public Manager
	{
	private:

		TextureManager() = default;
		std::vector<GLuint> ids;

	public:

		void Init()
		{
			auto const size = Manager::Size();
			ids.resize(size);
			if (size > 0)
			{
				glGenTextures(size, ids.data());
				OpenGL_LogError("glGenTexture");
			}
			SDL_verify(Update() == size);
		}

		void Free()
		{
			auto const size = Manager::Size();
			if (not ids.empty())
			{
				SDL_assert(ids.size() == size);
				glDeleteTextures(size, ids.data());
				OpenGL_LogError("glDeleteTextures");
				ids.clear();
			}
		}

		GLuint ID(unsigned index) const
		{
			return ids.at(index);
		}

		static TextureManager &Instance()
		{
			static TextureManager singleton;
			return singleton;
		}
	};
}

GLuint OpenGL_GetTexture(unsigned index)
{
	return TextureManager::Instance().ID(index);
}

void *OpenGL_GetContext(SDL_Window *window)
{
	static struct VideoContext
	{
		SDL_GLContext context = nullptr;

		VideoContext() = default;
		VideoContext(SDL_Window *window)
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
						SDL_perror("SDL_GL_MakeCurrent");
					}
					// Once only
					static bool init = false;
					if (not init)
					{
						init = true;
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
							SDL_perror("glewInit", string);
						}
					}
				}
				else
				{
					// Set error string and return null
					SDL_perror("SDL_GL_CreateContext");
					SDL_SetError(String(CannotCreateContext));
				}
			}
		}
		~VideoContext()
		{
			// Free if it was created
			if (context)
			{
				// Detach before deletion
				if (SDL_GL_GetCurrentContext() == context)
				{
					if (SDL_GL_MakeCurrent(nullptr, nullptr))
					{
						SDL_perror("SDL_GL_MakeCurrent");
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
		singleton = VideoContext(window);
	}
	return singleton.context;
}

// OpenGL error utility functions

signed OpenGL_SetError(const char *origin, GLenum error)
{
	return SDL_SetError("%s: %s", origin, gluErrorString(error));
}

signed OpenGL_SetError(const char *origin)
{
	GLenum error = glGetError();
	if (error)
	{
		return OpenGL_SetError(origin, error);
	}
	return 0;
}

signed OpenGL_LogError(const char *origin)
{
	GLenum error = glGetError();
	if (error)
	{
		union {
			const GLubyte *bytes;
			const char *string;
		};
		bytes = gluErrorString(error);
		return SDL_perror(origin, string);
	}
	return 0;
}
