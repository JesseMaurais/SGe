#include "FreeType.hpp"
#include "SDL.hpp"

namespace
{
	char const *ErrorString(FT_Error const error)
	{
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
		return "unknown";
	}
}

void FT::SetError(FT_Error const error)
{
	return SDL::SetError(ErrorString(error));
}

static FT_Library library;

static struct FreeType
{
	FreeType()
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

	~FreeType()
	{
		FT_Error const error = FT_Done_FreeType(library);
		if (error) 
		{
			if (SDL::SetError(error))
			{
				FT::perror("FT_Done_FreeType");
			}
		}
	}

} FT;

bool FontStruct::Load(const char *path)
{
	FT_Error const error = FT_New_Face(library, path, 0, &face);
	if (error)
	{
		if (FT::SetError(error))
		{
			SDL::perror("FT_New_Face");
		}
	}
	return not error;
}

void FontStruct::Free()
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


