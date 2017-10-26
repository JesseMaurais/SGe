#include "OpenAL.hpp"
#include "Manager.hpp"
#include "Source.hpp"
#include "Audio.hpp"
#include "SDL.hpp"
#include <vector>

// OpenAL buffer management

namespace
{
	class AudioBufferManager final : public Manager
	{
	private:

		AudioBufferManager() = default;
		std::vector<ALuint> ids;

	public:

		void Init()
		{
			auto const size = Manager::Size();
			ids.resize(size);
			if (size > 0)
			{
				alGenBuffers(size, ids.data());
				OpenAL_LogError("alGenBuffers");
			}
			SDL_verify(Update() == size);
		}

		void Free()
		{
			auto const size = Manager::Size();
			if (not ids.empty())
			{
				SDL_assert(ids.size() == size);
				alDeleteBuffers(size, ids.data());
				OpenAL_LogError("alDeleteBuffers");
				ids.clear();
			}
		}

		ALuint ID(unsigned index) const
		{
			return ids.at(index);
		}

		static AudioBufferManager &Instance()
		{
			static AudioBufferManager singleton;
			return singleton;
		}
	};
}

Resource &AudioBuffer::Manager()
{
	return AudioBufferManager::Instance();
}

ALuint OpenAL_GetBuffer(unsigned index)
{
	return AudioBufferManager::Instance().ID(index);
}

// OpenAL source management

namespace
{
	class AudioSourceManager final : public Manager
	{
	public:

		AudioSourceManager() = default;
		std::vector<ALuint> ids;

	public:

		void Init()
		{
			auto const size = Manager::Size();
			ids.resize(size);
			if (size > 0)
			{
				alGenSources(size, ids.data());
				OpenAL_LogError("alGenSources");
			}
			SDL_verify(Update() == size);
		}

		void Free()
		{
			auto const size = Manager::Size();
			if (not ids.empty())
			{
				SDL_assert(ids.size() == size);
				alDeleteSources(size, ids.data());
				OpenAL_LogError("alDeleteSources");
			}
		}

		ALuint ID(unsigned index) const
		{
			return ids.at(index);
		}

		static AudioSourceManager &Instance()
		{
			static AudioSourceManager singleton;
			return singleton;
		}
	};
}

Resource &AudioSource::Manager()
{
	return AudioSourceManager::Instance();
}

ALuint OpenAL_GetSource(unsigned index)
{
	return AudioSourceManager::Instance().ID(index);
}

// OpenAL device management

ALCdevice *OpenAL_GetDevice(const char *name)
{
	static struct AudioDevice
	{
		ALCdevice *device = nullptr;

		AudioDevice() = default;
		AudioDevice(const char *name)
		{
			if (name)
			{
				// No named device
				if ('\0' == *name)
				{
					// Use default
					name = nullptr;
				}
				// Open the named device
				device = alcOpenDevice(name);
				if (not device)
				{
					// Set the error string and return null
					OpenAL_SetError(nullptr, "alcOpenDevice");
				}
			}
		}
		~AudioDevice()
		{
			// Destroy the singleton context too
			SDL_verify(not OpenAL_GetContext(nullptr));
			// Close device if it was opened
			if (device and not alcCloseDevice(device))
			{
				OpenAL_LogError(nullptr, "alcCloseDevice");
			}
		}

	} singleton;
	// Either new or not initialized
	if (name or not singleton.device)
	{
		// Update to the named device
		singleton = AudioDevice(name);
	}
	return singleton.device;
}

// OpenAL context management

ALCcontext *OpenAL_GetContext(int const *attributes)
{
	static struct AudioContext
	{
		ALCcontext *context = nullptr;

		AudioContext() = default;
		AudioContext(int const *attributes)
		{
			if (attributes)
			{
				// No given attributes
				if (0 == *attributes)
				{
					// Use defaults
					attributes = nullptr;
				}
				// Get the singleton device first
				ALCdevice *device = OpenAL_GetDevice();
				if (device)
				{
					// Create a context with the given attributes
					context = alcCreateContext(device, attributes);
					if (not context)
					{
						// Set the error string and return null
						OpenAL_SetError(device, "alcCreateContext");
					}
					else
					{
						// Attach when creation succeeds
						if (not alcMakeContextCurrent(context))
						{
							OpenAL_LogError(device, "alcMakeContextCurrent");
						}
						// Generate audio buffers
						AudioBufferManager::Instance().Init();
						// Generate audio sources
						AudioSourceManager::Instance().Init();
						// Begin processing input
						alcProcessContext(context);
					}
				}
			}
		}
		~AudioContext()
		{
			// Destroy if it was created
			if (context)
			{
				// Stop processing input
				alcSuspendContext(context);
				// Delete audio sources
				AudioSourceManager::Instance().Free();
				// Delete audio buffers
				AudioBufferManager::Instance().Free();
				// Detach before we destroy it
				if (alcGetCurrentContext() == context)
				{
					if (not alcMakeContextCurrent(nullptr))
					{
						ALCdevice *device = alcGetContextsDevice(context);
						OpenAL_LogError(device, "alcMakeContextCurrent");
					}
				}
				// Now safe to destroy
				alcDestroyContext(context);
			}
		}

	} singleton;
	// Either new or not initialized
	if (attributes or not singleton.context)
	{
		// Update with given attributes
		singleton = AudioContext(attributes);
	}
	return singleton.context;
}

// OpenAL error utility functions

signed OpenAL_SetError(char const *origin, ALenum error)
{
	return SDL_SetError("%s: %s", origin, alGetString(error));
}

signed OpenAL_SetError(char const *origin)
{
	ALenum error = alGetError();
	if (error)
	{
		return OpenAL_SetError(origin, error);
	}
	return 0;
}

signed OpenAL_LogError(char const *origin)
{
	ALenum error = alGetError();
	if (error)
	{
		return SDL_perror(origin, alGetString(error));
	}
	return 0;
}

// ALC error utility functions

signed OpenAL_SetError(ALCdevice *device, char const *origin, ALenum error)
{
	return SDL_SetError("%s: %s", origin, alcGetString(device, error));
}

signed OpenAL_SetError(ALCdevice *device, char const *origin)
{
	ALCenum error = alcGetError(device);
	if (error)
	{
		return OpenAL_SetError(device, origin, error);
	}
	return 0;
}

signed OpenAL_LogError(ALCdevice *device, char const *origin)
{
	ALCenum error = alcGetError(device);
	if (error)
	{
		return SDL_perror(origin, alcGetString(device, error));
	}
	return 0;
}
