/** \file
 * Utility functions for working with SDL2.
 */

#ifndef SDL_hpp
#define SDL_hpp

#include <SDL2/SDL.h>

namespace SDL
{
	/// Initialize while also registering automatic cleanup
	bool Init(Uint32 const flags);

	/// Logs the current error string like std::perror does. Returns true when an error exists
	bool perror(char const *origin);

	/// Logs the given error string with origin as prefix. Always returns true.
	bool perror(char const *origin, char const *error);

	/// Message box with the current error. Returns true when retry requested
	bool ShowError(SDL_MessageBoxFlags flags, SDL_Window *transientFor = nullptr);

	/// Make SDL_assert activate a message box asking how to proceed
	void SetAssertionHandler(SDL_Window *transientFor = nullptr);

	/// Reset to the default error handler and flush all prior asserts
	void ResetAssertionHandler();

	/// Make assertion message box in scope transient for a window
	class ScopedAssertHandler
	{
	public:

		ScopedAssertHandler(SDL_Window *window = nullptr);
		~ScopedAssertHandler();

	private:

		SDL_AssertionHandler handler;
		void *userdata;
	};
}

#endif // file

