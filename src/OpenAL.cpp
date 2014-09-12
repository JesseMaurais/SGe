#include <vector>
#include <cstdlib>
#include "OpenAL.hpp"
#include "SDL.hpp"
#include "Lua.hpp"

static ALCdevice *Device;
static ALCcontext *Context;

signed OpenAL_Init()
{
	std::vector<ALCint> attributes;
	const char *string = getenv("AUDIODEVICE");

	lua_getglobal(State, "Audio");
	int table = lua_gettop(State);
	if (!lua_isnil(State, table))
	{
	 lua_pushnil(State);
	 while (lua_next(State, table))
	 {
		const char *key = lua_tostring(State, -2);
		const int value = lua_tointeger(State, -1);

		if (!SDL_strcasecmp(key, "SYNC"))
		{
		 attributes.push_back(ALC_SYNC);
		 attributes.push_back(value);
		}
		else
		if (!SDL_strcasecmp(key, "REFRESH"))
		{
		 attributes.push_back(ALC_REFRESH);
		 attributes.push_back(value);
		}
		else
		if (!SDL_strcasecmp(key, "FREQUENCY"))
		{
		 attributes.push_back(ALC_FREQUENCY);
		 attributes.push_back(value);
		}
		else
		if (!SDL_strcasecmp(key, "MONO_SOURCES"))
		{
		 attributes.push_back(ALC_MONO_SOURCES);
		 attributes.push_back(value);
		}
		else
		if (!SDL_strcasecmp(key, "STEREO_SOURCES"))
		{
		 attributes.push_back(ALC_STEREO_SOURCES);
		 attributes.push_back(value);
		}

		lua_pop(State, 1);
	 }
	}
	lua_pop(State, 1);

	Device = alcOpenDevice(string);
	if (!Device)
	{
	 ALCenum error = alcGetError(NULL);
	 string = alcGetString(NULL, error);
	 return SDL_SetError("alcOpenDevice: %s", string);
	}

	attributes.push_back(0);

	Context = alcCreateContext(Device, attributes.data());
	if (!Context)
	{
	 ALCenum error = alcGetError(Device);
	 string = alcGetString(Device, error);
	 alcCloseDevice(Device);
	 return SDL_SetError("alcCreateContext: %s", string);
	}

	alcMakeContextCurrent(Context);
	alcProcessContext(Context);
	return 0;
}

void OpenAL_Quit()
{
	alcDestroyContext(Context);
	alcCloseDevice(Device);
}

