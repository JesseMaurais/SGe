#include "Audio.hpp"
#include "OpenAL.hpp"
#include "Manager.hpp"
#include "Event.hpp"
#include "SDL.hpp"
#include "std.hpp"

// Resource management implementation

namespace
{
	// User event codes to queue updates for buffers and sources
	enum AudioEventCode : Uint32 { UpdateBuffers, UpdateSources };

	// Generic resource manager for any ALuint based id type
	template <AudioEventCode UpdateCode>
	class AudioManager : public UpdateManager<ALuint>
	{
	private:

		bool SendUpdate() override
		{
			return SendUserEvent(UpdateAudio, UpdateCode);
		}
	};

	class AudioBufferManager final : public AudioManager<UpdateBuffers>
	{
	public:

		static AudioBufferManager &Instance()
		{
			static AudioBufferManager singleton;
			return singleton;
		}

	private:

		AudioBufferManager() = default;

		void Generate(std::vector<ALuint> &ids) override
		{
			alGenBuffers(ids.size(), ids.data());
			OpenAL_LogError("alGenBuffers");
		}

		void Destroy(std::vector<ALuint> &ids) override
		{
			alDeleteBuffers(ids.size(), ids.data());
			OpenAL_LogError("alDeleteBuffers");
		}
	};

	class AudioSourceManager final : public AudioManager<UpdateSources>
	{
	public:

		static AudioSourceManager &Instance()
		{
			static AudioSourceManager singleton;
			return singleton;
		}

	private:

		AudioSourceManager() = default;

		void Generate(std::vector<ALuint> &ids) override
		{
			alGenSources(ids.size(), ids.data());
			OpenAL_LogError("alGenSources");
		}

		void Destroy(std::vector<ALuint> &ids) override
		{
			alDeleteSources(ids.size(), ids.data());
			OpenAL_LogError("alDeleteSources");
		}
	};
}

// OpenAL buffer management

Resources &AudioBuffer::Manager()
{
	return AudioBufferManager::Instance();
}

ALuint OpenAL_GetBuffer(unsigned index)
{
	return AudioBufferManager::Instance().ID(index);
}

// OpenAL source management

Resources &AudioSource::Manager()
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
	static class AudioDevice
	{
	public:

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
			verify(not OpenAL_GetContext(nullptr));
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
	static class AudioContext
	{
	public:

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
						AudioBufferManager::Instance().Initialize();
						// Generate audio sources
						AudioSourceManager::Instance().Initialize();
						// Begin processing input
						alcProcessContext(context);
						// Register update handler
						PushEventHandler(UpdateAudio, UpdateAudioHandler);
					}
				}
			}
		}
		~AudioContext()
		{
			// Destroy if it was created
			if (context)
			{
				// Unregister update handler
				PopEventHandler(UpdateAudio);
				// Stop processing input
				alcSuspendContext(context);
				// Delete audio sources
				AudioSourceManager::Instance().Release();
				// Delete audio buffers
				AudioBufferManager::Instance().Release();
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

	private:

		static bool UpdateAudioHandler(SDL_Event const &event)
		{
			assert(UserEventType::UpdateAudio == event.user.type);
			switch (event.user.code)
			{
			case AudioEventCode::UpdateBuffers:
				AudioBufferManager::Instance().Update();
				break;
			case AudioEventCode::UpdateSources:
				AudioSourceManager::Instance().Update();
				break;
			default:
				assert(not "AudioEventCode");
				return false;
			}
			return true;
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
