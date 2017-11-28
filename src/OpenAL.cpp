#include "OpenAL.hpp"
#include "Manager.hpp"
#include "Audio.hpp"
#include "Event.hpp"
#include "Error.hpp"
#include "SDL.hpp"

// Resource management implementation

namespace
{
	// User event codes to queue updates for buffers and sources
	enum UpdateEventCode : Uint32 { UpdateBuffers, UpdateSources };

	// Generic resource manager for any ALuint based id type
	template <UpdateEventCode UpdateCode>
	class UpdateManager : public Manager<ALuint>
	{
	private:

		bool SendUpdate() override
		{
			return SDL::SendUserEvent(UpdateOpenAL, UpdateCode);
		}
	};

	class AudioBufferManager final : public UpdateManager<UpdateBuffers>
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
			if (OpenAL::CheckError("alGenBuffers"))
			{
				SDL::LogError(CannotAllocateResource);
			}
		}

		void Destroy(std::vector<ALuint> const &ids) override
		{
			alDeleteBuffers(ids.size(), ids.data());
			if (OpenAL::LogError("alDeleteBuffers"))
			{
				SDL::LogError(CannotFreeResource);
			}
		}
	};

	class AudioSourceManager final : public UpdateManager<UpdateSources>
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
			if (OpenAL::CheckError("alGenSources"))
			{
				SDL::LogError(CannotAllocateResource);
			}
		}

		void Destroy(std::vector<ALuint> const &ids) override
		{
			alDeleteSources(ids.size(), ids.data());
			if (OpenAL::CheckError("alDeleteSources"))
			{
				SDL::LogError(CannotFreeResource);
			}
		}
	};
}

// OpenAL buffer management

Resources &AudioBuffer::Manager()
{
	return AudioBufferManager::Instance();
}

ALuint OpenAL::GetBuffer(unsigned index)
{
	return AudioBufferManager::Instance().Data(index);
}

// OpenAL source management

Resources &AudioSource::Manager()
{
	return AudioSourceManager::Instance();
}

ALuint OpenAL::GetSource(unsigned index)
{
	return AudioSourceManager::Instance().Data(index);
}

// OpenAL device management

ALCdevice *OpenAL::GetDevice(const char *name)
{
	static class Device
	{
	public:

		ALCdevice *device = nullptr;

		Device() = default;
		Device(const char *name)
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
					OpenAL::CheckError(nullptr, "alcOpenDevice");
				}
			}
		}
		~Device()
		{
			// Destroy the singleton context too
			verify(not OpenAL::GetContext(nullptr));
			// Close device if it was opened
			if (device and not alcCloseDevice(device))
			{
				OpenAL::LogError(nullptr, "alcCloseDevice");
			}
		}

	} singleton;
	// Either new or not initialized
	if (name or not singleton.device)
	{
		// Update to the named device
		singleton = Device(name);
	}
	return singleton.device;
}

// OpenAL context management

ALCcontext *OpenAL::GetContext(int const *attributes)
{
	static class Context
	{
	private:

		static void Initialize()
		{
			// Generate audio buffers
			AudioBufferManager::Instance().Initialize();
			// Generate audio sources
			AudioSourceManager::Instance().Initialize();
		}

		static void Release()
		{
			// Delete audio sources
			AudioSourceManager::Instance().Release();
			// Delete audio buffers
			AudioBufferManager::Instance().Release();
		}

		static void UpdateHandler(SDL_Event const &event)
		{
			switch (event.user.code)
			{
			case UpdateBuffers:
				AudioBufferManager::Instance().Update();
				break;
			case UpdateSources:
				AudioSourceManager::Instance().Update();
				break;
			default:
				assert(not "OpenAL event code");
			}
		}

		ScopedEventHandler updater;

	public:

		ALCcontext *context = nullptr;

		Context() = default;
		Context(int const *attributes)
		: updater(SDL::UserEvent(UpdateOpenAL), UpdateHandler)
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
				ALCdevice *device = OpenAL::GetDevice();
				if (device)
				{
					// Create a context with the given attributes
					context = alcCreateContext(device, attributes);
					if (not context)
					{
						// Set the error string and return null
						OpenAL::CheckError(device, "alcCreateContext");
					}
					else
					{
						// Attach when creation succeeds
						if (not alcMakeContextCurrent(context))
						{
							OpenAL::LogError(device, "alcMakeContextCurrent");
						}
						Initialize();
						// Begin processing input
						alcProcessContext(context);
					}
				}
			}
		}
		~Context()
		{
			// Destroy if it was created
			if (context)
			{
				// Stop processing input
				alcSuspendContext(context);
				// Detach before we destroy it
				if (alcGetCurrentContext() == context)
				{
					if (not alcMakeContextCurrent(nullptr))
					{
						ALCdevice *device = alcGetContextsDevice(context);
						OpenAL::LogError(device, "alcMakeContextCurrent");
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
		singleton = Context(attributes);
	}
	return singleton.context;
}

// OpenAL error utility functions

bool OpenAL::SetError(const char *origin, ALenum error)
{
	return SDL::SetError(ColonSeparator, origin, alGetString(error));
}

bool OpenAL::CheckError(const char *origin)
{
	ALenum const error = alGetError();
	return error or OpenAL::SetError(origin, error);
}

bool OpenAL::LogError(const char *origin)
{
	ALenum const error = alGetError();
	return error or SDL::LogError(origin, alGetString(error));
}

// ALC error utility functions

bool OpenAL::SetError(ALCdevice *device, const char *origin, ALenum error)
{
	return SDL::SetError(ColonSeparator, origin, alcGetString(device, error));
}

bool OpenAL::CheckError(ALCdevice *device, const char *origin)
{
	ALCenum const error = alcGetError(device);
	return error or OpenAL::SetError(device, origin, error);
}

bool OpenAL::LogError(ALCdevice *device, const char *origin)
{
	ALCenum const error = alcGetError(device);
	return error or SDL::LogError(origin, alcGetString(device, error));
}
