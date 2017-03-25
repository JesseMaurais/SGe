#ifndef OpenAL_hpp
#define OpenAL_hpp

#include <AL/alc.h>
#include <AL/al.h>

/// Set the SDL error string to the given error code
signed OpenAL_SetError(const char *origin, ALenum error);
/// Set the SDL error string to the current error code
signed OpenAL_SetError(const char *origin);
/// Log the current error with SDL
signed OpenAL_LogError(const char *origin);

/// Set the SDL error string to the given device error code
signed OpenAL_SetError(ALCdevice *device, const char *origin, ALCenum error);
/// Set the SDL error string to the current device error code
signed OpenAL_SetError(ALCdevice *device, const char *origin);
/// Log the current device error with SDL
signed OpenAL_LogError(ALCdevice *device, const char *origin);

/**
 * Update the audio device singleton for the given device name.
 * @param name The name of the device to be opened or null for the default.
 * @return The named device handle, or null and the error string is set.
 */
ALCdevice *OpenAL_GetDevice(const char *name = nullptr);

/**
 * Update the audio context singleton with the given attributes.
 * @param attributes The audio context attributes to use or null for defaults.
 * @return Context handle now current, or null and the error string is set.
 */
ALCcontext *OpenAL_GetContext(const int *attributes = nullptr);

#endif // file

