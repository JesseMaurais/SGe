#include "OpenAL.hpp"
#include "Strings.hpp"
#include "SDL.hpp"


ALCdevice *OpenAL_GetDevice(const char *name)
{
	static struct AudioDevice
	{
		ALCdevice *device = nullptr;
		AudioDevice() = default;
		AudioDevice(const char *name)
		{
			// Open the named device
			device = alcOpenDevice(name);
			if (not device)
			{
				// Set the error string and return null
				OpenAL_SetError(nullptr, "alcOpenDevice");
			}
		}
		~AudioDevice()
		{
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


ALCcontext *OpenAL_GetContext(const int *attributes)
{
	static struct AudioContext
	{
		ALCcontext *context = nullptr;
		AudioContext() = default;
		AudioContext(const int *attributes)
		{
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
					alcProcessContext(context);
				}
			}
		}
		~AudioContext()
		{
			// Destroy if it was created
			if (context)
			{
				alcSuspendContext(context);
				// Detach before we destroy it
				if (alcGetCurrentContext() == context)
				{
					if (not alcMakeContextCurrent(nullptr))
					{
						ALCdevice *device = alcGetContextsDevice(context);
						OpenAL_LogError(device, "alcMakeContextCurrent");
					}
				}
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

signed OpenAL_SetError(const char *origin, ALenum error)
{
	return SDL_SetError("%s: %s", origin, alGetString(error));
}

signed OpenAL_SetError(const char *origin)
{
	ALenum error = alGetError();
	if (error)
	{
		return OpenAL_SetError(origin, error);
	}
	return 0;
}

signed OpenAL_LogError(const char *origin)
{
	ALenum error = alGetError();
	if (error)
	{
		return SDL_perror(origin, alGetString(error));
	}
	return 0;
}

// ALC error utility functions

signed OpenAL_SetError(ALCdevice *device, const char *origin, ALenum error)
{
	return SDL_SetError("%s: %s", origin, alcGetString(device, error));
}

signed OpenAL_SetError(ALCdevice *device, const char *origin)
{
	ALCenum error = alcGetError(device);
	if (error)
	{
		return OpenAL_SetError(device, origin, error);
	}
	return 0;
}

signed OpenAL_LogError(ALCdevice *device, const char *origin)
{
	ALCenum error = alcGetError(device);
	if (error)
	{
		return SDL_perror(origin, alcGetString(device, error));
	}
	return 0;
}
