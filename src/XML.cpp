#include "XML.hpp"
#include "Lua.hpp"
#include "SDL.hpp"
#include <cstdlib>
#include <cerrno>

static int eval(const char *string)
{
	switch (luaL_loadstring(State, string))
	{
	  case LUA_OK:
		break;
	  case LUA_ERRMEM:
		return SDL_SetError("out of memory");
	  case LUA_ERRGCMM:
		return SDL_SetError("gc method fault");
	  case LUA_ERRSYNTAX:
		return SDL_SetError("syntax error");
	}
	switch (lua_pcall(State, 0, 1, 0))
	{
	  case LUA_OK:
		break;
	  case LUA_ERRMEM:
		return SDL_SetError("out of memory");
	  case LUA_ERRGCMM:
		return SDL_SetError("gc method fault");
	  case LUA_ERRRUN:
		{
		const char *error = lua_tostring(State, -1);
		int code = SDL_SetError("%s", error);
		lua_pop(State, 1);
		return code;
		}
	}
	return 0;
}

XML::XML(const char *encoding)
{
	parser = XML_ParserCreate(encoding);
	assert(parser);
	XML_SetElementHandler(parser, start, end);
	XML_SetCharacterDataHandler(parser, cdata);
	XML_SetUserData(parser, this);
}

XML::~XML(void)
{
	XML_ParserFree(parser);
}

void XML::start(void *user, const char *name, const char **attributes)
{
	union {
	 void *addr;
	 XML *obj;
	};
	addr = user;
	obj->Start(name, attributes);
}

void XML::end(void *user, const char *name)
{
	union {
	 void *addr;
	 XML *obj;
	};
	addr = user;
	obj->End(name);
}

void XML::cdata(void *user, const char *string, int length)
{
	union {
	 void *addr;
	 XML *obj;
	};
	addr = user;
	obj->CData(string, length);
}

bool XML::Load(SDL_RWops *ops)
{
	void *buffer;
	size_t size;
	do
	{
		buffer = XML_GetBuffer(parser, BUFSIZ);
		size = SDL_RWread(ops, buffer, sizeof(char), BUFSIZ);
		if (!XML_ParseBuffer(parser, size, 0 == size))
		{
			XML_Error code = XML_GetErrorCode(parser);
			const char *string = XML_ErrorString(code);
			int line = XML_GetCurrentLineNumber(parser);
			int col = XML_GetCurrentColumnNumber(parser);
			SDL_Log("XML (%d, %d) %s", line, col, string);
			return false;
		}
	}
	while (size > 0);
	return true;
}

bool XML::Load(const char *path)
{
	SDL_RWops *ops = SDL_RWFromFile(path, "r");
	if (!ops) SDL_perror("SDL_RWFromFile");
	else
	{
	 bool okay = Load(ops);
	 SDL_RWclose(ops);
	 return okay;
	}
	return false;
}

void XML::Start(const char *name, const char **attributes)
{
	lua_getfield(State, -1, "child");
	lua_len(State, -1);

	lua_newtable(State);
	luaL_setmetatable(State, name);
	for (int key=0, value=1; attributes[key]; key += 2, value += 2)
	{
		lua_pushstring(State, attributes[key]);
		lua_pushstring(State, attributes[value]);
		lua_settable(State, -3);
	}
}

void XML::End(const char *name)
{
	lua_settable(State, -3);
	lua_pop(State, 1);
}

void XML::CData(const char *string, int length)
{

}

