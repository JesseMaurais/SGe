#include "XML.hpp"
#include "SDL.hpp"
#include "Strings.hpp"
#include <stdexcept>
#include <expat.h>

XML::XML(const char *encoding)
{
	auto start = [](void *user, const char *name, const char **attributes)
	{
		union {
		 void *addr;
		 XML *obj;
		};
		addr = user;
		obj->Start(name, attributes);
	};

	auto end = [](void *user, const char *name)
	{
		union {
		 void *addr;
		 XML *obj;
		};
		addr = user;
		obj->End(name);
	};

	parser = XML_ParserCreate(encoding);
	if (not parser) throw std::runtime_error(CannotCreateParser);
	XML_SetElementHandler(parser, start, end);
	XML_SetUserData(parser, this);
}

XML::~XML(void)
{
	if (parser) XML_ParserFree(parser);
}

bool XML::Load(SDL_RWops *ops)
{
	void *buffer;
	std::size_t size;
	do
	{
		buffer = XML_GetBuffer(parser, BUFSIZ);
		size = SDL_RWread(ops, buffer, sizeof(char), BUFSIZ);
		if (not XML_ParseBuffer(parser, size, 0 == size))
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
	if (ops)
	{
		bool okay = Load(ops);
		if (SDL_RWclose(ops))
		{
			SDL_perror(path);
		}
		return okay;
	}
	SDL_perror(path);
	return false;
}

