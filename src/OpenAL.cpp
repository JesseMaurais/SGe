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
				SDL::perror(CannotCreateResource);
			}
		}

		void Destroy(std::vector<ALuint> const &ids) override
		{
			alDeleteBuffers(ids.size(), ids.data());
			if (OpenAL::LogError("alDeleteBuffers"))
			{
				SDL::perror(CannotDeleteResource);
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
				SDL::perror(CannotCreateResource);
			}
		}

		void Destroy(std::vector<ALuint> const &ids) override
		{
			alDeleteSources(ids.size(), ids.data());
			if (OpenAL::LogError("alDeleteSources"))
			{
				SDL::perror(CannotDeleteResource);
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
	public:

		ALCcontext *context = nullptr;

		Context() = default;
		Context(int const *attributes)
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
						// Generate audio buffers
						AudioBufferManager::Instance().Initialize();
						// Generate audio sources
						AudioSourceManager::Instance().Initialize();
						// Begin processing input
						alcProcessContext(context);
						// Register update handler
						SDL::PushEventHandler(UpdateOpenAL, UpdateHandler);
					}
				}
			}
		}
		~Context()
		{
			// Destroy if it was created
			if (context)
			{
				// Unregister update handler
				SDL::PopEventHandler(UpdateOpenAL);
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
						OpenAL::LogError(device, "alcMakeContextCurrent");
					}
				}
				// Now safe to destroy
				alcDestroyContext(context);
			}
		}

	private:

		static bool UpdateHandler(SDL_Event const &event)
		{
			assert(UserEventType::UpdateOpenAL == event.user.type);
			// Update method by code
			switch (event.user.code)
			{
			case UpdateEventCode::UpdateBuffers:
				AudioBufferManager::Instance().Update();
				break;
			case UpdateEventCode::UpdateSources:
				AudioSourceManager::Instance().Update();
				break;
			default:
				assert(not "OpenAL event code");
				return false;
			}
			return true;
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
	if (error)
	{
		return OpenAL::SetError(origin, error);
	}
	return false;
}

bool OpenAL::LogError(const char *origin)
{
	ALenum const error = alGetError();
	if (error)
	{
		return SDL::perror(origin, alGetString(error));
	}
	return false;
}

// ALC error utility functions

signed OpenAL::SetError(ALCdevice *device, char const *origin, ALenum error)
{
	return SDL::SetError(ColonSeparator, origin, alcGetString(device, error));
}

signed OpenAL::CheckError(ALCdevice *device, char const *origin)
{
	ALCenum const error = alcGetError(device);
	if (error)
	{
		return OpenAL::SetError(device, origin, error);
	}
	return 0;
}

signed OpenAL::LogError(ALCdevice *device, char const *origin)
{
	ALCenum const error = alcGetError(device);
	if (error)
	{
		return SDL::perror(origin, alcGetString(device, error));
	}
	return 0;
}
