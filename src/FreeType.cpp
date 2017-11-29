#include "FreeType.hpp"
#include "Error.hpp"

namespace
{
	char const *ErrorString(FT_Error const error)
	{
		#ifndef __CDT_PARSER__
		#undef __FTERRORS_H__
		#define FT_ERRORDEF(e, v, s) { e, s },
		#define FT_ERROR_START_LIST {
		#define FT_ERROR_END_LIST { 0, NULL } };
		const struct
		{
			int error;
			char const *string;
		} list[] =
		#include <freetype2/freetype/fterrors.h>
		for (unsigned it = 0; list[it].error; ++it)
		{
			if (error == list[it].error)
			{
				return list[it].string;
			}
		}
		#endif // CDT PARSER
		return "unknown";
	}
}

bool FT::SetError(FT_Error const error)
{
	return SDL::SetError(ErrorString(error));
}

static struct FreeTypeClass
{
	FT_Library library;

	FreeTypeClass()
	{
		FT_Error const error = FT_Init_FreeType(&library);
		if (error)
		{
			if (FT::SetError(error))
			{
				SDL::perror("FT_InitFreeType");
			}
		}
	}
	~FreeTypeClass()
	{
		FT_Error const error = FT_Done_FreeType(library);
		if (error) 
		{
			if (FT::SetError(error))
			{
				SDL::perror("FT_Done_FreeType");
			}
		}
	}

} FreeType;

FT::FontStruct::FontStruct(std::string const &path)
{
	FT_Error const error = FT_New_Face(FreeType.library, path.c_str(), 0, &face);
	if (error)
	{
		if (FT::SetError(error))
		{
			SDL::perror("FT_New_Face");
		}
	}
}

FT::FontStruct::~FontStruct()
{
	FT_Error const error = FT_Done_Face(face);
	if (error)
	{
		if (FT::SetError(error))
		{
			SDL::perror("FT_Done_Face");
		}
	}
}


