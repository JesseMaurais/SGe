#include "SDL.hpp"
#include "Lua.hpp"
#include "OpenAL.hpp"
#include <vector>

static ALCdevice *Device;
static ALCcontext *Context;

signed OpenAL_Init()
{
	std::vector<ALCint> attributes;
	const char *string = nullptr;

	lua_getglobal(State, "Audio");
	int table = lua_gettop(State);
	if (!lua_isnil(State, table))
	{
	 lua_pushnil(State);
	 while (lua_next(State, table))
	 {
		const char *key = lua_tostring(State, -2);
		#define tointeger lua_tointeger(State, -1)
		#define toboolean lua_toboolean(State, -1)
		#define tostring  lua_tostring(State, -1)

		if (!SDL_strcasecmp(key, "SYNC"))
		{
		 attributes.push_back(ALC_SYNC);
		 attributes.push_back(toboolean);
		}
		else
		if (!SDL_strcasecmp(key, "REFRESH"))
		{
		 attributes.push_back(ALC_REFRESH);
		 attributes.push_back(tointeger);
		}
		else
		if (!SDL_strcasecmp(key, "FREQUENCY"))
		{
		 attributes.push_back(ALC_FREQUENCY);
		 attributes.push_back(tointeger);
		}
		else
		if (!SDL_strcasecmp(key, "MONO_SOURCES"))
		{
		 attributes.push_back(ALC_MONO_SOURCES);
		 attributes.push_back(tointeger);
		}
		else
		if (!SDL_strcasecmp(key, "STEREO_SOURCES"))
		{
		 attributes.push_back(ALC_STEREO_SOURCES);
		 attributes.push_back(tointeger);
		}
		else
		if (!SDL_strcasecmp(key, "DEVICE"))
		{
		 string = tostring;
		}
		else
		{
		 SDL_Log("Audio: %s does not match", key);
		}

		lua_pop(State, 1);
	 }
	}
	lua_pop(State, 1);

	Device = alcOpenDevice(string);
	if (!Device)
	{
	 ALCenum error = alcGetError(nullptr);
	 string = alcGetString(nullptr, error);
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

