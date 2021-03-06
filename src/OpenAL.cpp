#include "OpenAL.hpp"
#include "Manager.hpp"
#include "Event.hpp"
#include "Error.hpp"
#include "SDL.hpp"
#include "assert.hpp"

// Resource management implementation

namespace
{
	// User event codes to queue updates for buffers and sources
	enum UpdateEventCode : Uint32 { UpdateBuffers, UpdateSources };

	// Generic resource manager for any ALuint based id type
	template <UpdateEventCode UpdateCode>
	class UpdateManager : public Manager<Slot<ALuint>*, ALuint>
	{
	private:

		bool SendUpdate() const override
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

		void Generate(std::vector<ALuint> &ids) const override
		{
			alGenBuffers(ids.size(), ids.data());
			if (OpenAL::CheckError("alGenBuffers"))
			{
				SDL::LogError(CannotAllocateResource);
			}
		}

		void Destroy(std::vector<ALuint> const &ids) const override
		{
			alDeleteBuffers(ids.size(), ids.data());
			if (OpenAL::CheckError("alDeleteBuffers"))
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

		void Generate(std::vector<ALuint> &ids) const override
		{
			alGenSources(ids.size(), ids.data());
			if (OpenAL::CheckError("alGenSources"))
			{
				SDL::LogError(CannotAllocateResource);
			}
		}

		void Destroy(std::vector<ALuint> const &ids) const override
		{
			alDeleteSources(ids.size(), ids.data());
			if (OpenAL::CheckError("alDeleteSources"))
			{
				SDL::LogError(CannotFreeResource);
			}
		}
	};
}

// Bind observers to the correct manager

OpenAL::Buffer::Buffer(Observer observer)
: Slot(AudioBufferManager::Instance(), observer)
{}

OpenAL::Source::Source(Observer observer)
: Slot(AudioSourceManager::Instance(), observer)
{}

// Device singleton

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
	// Update to the named device
	singleton = Device(name);
	return singleton.device;
}

// Context singleton

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

		unsigned UpdateEvent = SDL::UserEvent(UpdateOpenAL);
		ScopedEventHandler updater{ UpdateEvent, UpdateHandler };

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
	// Update with given attributes
	singleton = Context(attributes);
	return singleton.context;
}

// Error utility functions

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
