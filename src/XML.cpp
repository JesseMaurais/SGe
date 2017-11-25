#include "XML.hpp"
#include "SDL.hpp"
#include "Error.hpp"
#include <stdexcept>

XML::XML(char const *encoding)
{
	auto start = [](void *user, char const *name, char const **attributes)
	{
		union {
		 void *address;
		 XML *object;
		};
		address = user;
		object->Start(name, attributes);
	};

	auto end = [](void *user, char const *name)
	{
		union {
		 void *address;
		 XML *object;
		};
		address = user;
		object->End(name);
	};

	parser = XML_ParserCreate(encoding);
	if (not parser) throw std::runtime_error(String(CannotCreateParser));
	XML_SetElementHandler(parser, start, end);
	XML_SetUserData(parser, this);
}

XML::~XML(void)
{
	if (parser) XML_ParserFree(parser);
}

bool XML::Parse(SDL_RWops *ops)
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
			char const *string = XML_ErrorString(code);
			int line = XML_GetCurrentLineNumber(parser);
			int col = XML_GetCurrentColumnNumber(parser);
			SDL::SetError("XML (%1, %2) %3", line, col, string);
			return false;
		}
	}
	while (size > 0);
	return true;
}
