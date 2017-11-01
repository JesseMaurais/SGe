/** \file
 * Utilities for with SDL2
 * 		Additional error string reporting functions
 */

#ifndef SDL_hpp
#define SDL_hpp

#include <SDL2/SDL.h>

namespace SDL
{
	/**
	 * \brief Setup a handler so that SDL_assert activates a popup window with
	 * options about how to proceed.
	 * \param window Handle to the window that the popup is transient for, or
	 * null so that it floats over the desktop.
	 */
	void SetAssertHandler(SDL_Window *window = nullptr);
	/**
	 * \brief Reset to the default error handler and flush all prior asserts.
	 */
	void ResetAssertHandler();

	/**
	 * \brief Logs the current error string like 'perror' does.
	 * \param origin Name of the function which generated the error.
	 * \return If there is no error, 0, otherwise -1.
	 */
	signed perror(const char *origin);

	/**
	 * \brief Logs the given error string with origin as prefix.
	 * \param origin Name of the function which generated the error.
	 * \return Always returns -1.
	 */
	signed perror(const char *origin, const char *error);
}

#endif // file

