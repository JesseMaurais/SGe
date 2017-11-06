/** \file
 * Utility functions for working with OpenAL.
 */

#ifndef OpenAL_hpp
#define OpenAL_hpp

#include <AL/alc.h>
#include <AL/al.h>

namespace OpenAL
{
	/// Set the SDL error string to the given error code. Always returns true.
	bool SetError(const char *origin, ALenum error);
	/// Set the SDL error string to the current error code. Returns true when an error exists.
	bool CheckError(const char *origin);
	/// Log the current error with SDL. Returns true when an error exists.
	bool LogError(const char *origin);

	/// Set the SDL error string to the given device error code. Always returns true.
	bool SetError(ALCdevice *device, const char *origin, ALCenum error);
	/// Set the SDL error string to the current device error code. Returns true when an error exists.
	bool CheckError(ALCdevice *device, const char *origin);
	/// Log the current device error with SDL. Returns true when an error exists.
	bool LogError(ALCdevice *device, const char *origin);

	// Buffer id at the given index from within the singleton.
	ALuint GetBuffer(unsigned index);
	// Source id at the given index from within the singleton.
	ALuint GetSource(unsigned index);

	/**
	 * \brief Update the audio device singleton for the given device name.
	 * \param name The name of the device to be opened or none for the default.
	 * Provide null to close the singleton device. It will be closed automatically
	 * at exit.
	 * \return The named device handle, or null and the error string is set.
	 */
	ALCdevice *GetDevice(const char *name = "\0");

	/**
	 * \brief Update the audio context singleton with the given attributes.
	 * \param attributes The audio context attributes to use or none for defaults.
	 * Provide null to destroy the singleton context. It will be destroyed
	 * automatically at exit.
	 * \return Context handle now current, or null and the error string is set.
	 */
	ALCcontext *GetContext(const int *attributes = {0});
}

#endif // file

