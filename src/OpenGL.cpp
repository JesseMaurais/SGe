#include "OpenGL.hpp"
#include "Manager.hpp"
#include "Strings.hpp"
#include "Event.hpp"
#include "SDL.hpp"

namespace
{
	enum VideoEventCode : Uint32 { UpdateTextures };

	template <VideoEventCode UpdateCode>
	class VideoManager : public Manager<GLuint>
	{
	private:

		bool SendUpdate() override
		{
			return SendUserEvent(UpdateVideo, UpdateCode);
		}
	};

	class TextureManager final : public VideoManager<UpdateTextures>
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
			OpenGL::CheckError("glGenTextures");
		}

		void Destroy(std::vector<GLuint> const &ids) override
		{
			glDeleteTextures(ids.size(), ids.data());
			OpenGL::CheckError("glDeleteTextures");
		}
	};
}

GLuint OpenGL::GetTexture(unsigned index)
{
	return TextureManager::Instance().Data(index);
}

void *OpenGL::GetContext(SDL_Window *window)
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
						SDL::perror("SDL_GL_MakeCurrent");
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
							SDL::perror("glewInit", string);
						}
						else
						{
							// Register update handler
							PushEventHandler(UpdateVideo, UpdateVideoHandler);
						}
					}
				}
				else
				{
					// Set error string and return null
					SDL::perror("SDL_GL_CreateContext");
					SDL_SetError(String(CannotCreateContext));
				}
			}
		}
		~VideoContext()
		{
			// Free if it was created
			if (context)
			{
				// Unregister event handler
				PopEventHandler(UpdateVideo);
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

		static bool UpdateVideoHandler(SDL_Event const &event)
		{
			assert(UserEventType::UpdateVideo == event.user.type);
			switch (event.user.code)
			{
			case VideoEventCode::UpdateTextures:
				TextureManager::Instance().Update();
				break;
			default:
				assert(not "VideoEventCode");
				return false;
			}
			return true;
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

signed OpenGL::SetError(const char *origin, GLenum error)
{
	return SDL_SetError("%s: %s", origin, gluErrorString(error));
}

signed OpenGL::CheckError(const char *origin)
{
	GLenum error = glGetError();
	if (error)
	{
		return OpenGL::SetError(origin, error);
	}
	return 0;
}

signed OpenGL::LogError(const char *origin)
{
	GLenum error = glGetError();
	if (error)
	{
		union {
			const GLubyte *bytes;
			const char *string;
		};
		bytes = gluErrorString(error);
		return SDL::perror(origin, string);
	}
	return 0;
}
