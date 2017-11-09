#include "FreeType.hpp"
#include <SDL2/SDL.h>
#include <cassert>

static const char *ErrorString(FT_Error error)
{
	#undef __FTERRORS_H__
	#define FT_ERRORDEF(e, v, s) { e, s },
	#define FT_ERROR_START_LIST {
	#define FT_ERROR_END_LIST { 0, NULL } };
	const struct
	{
	 int error;
	 const char *string;
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

void FT_perror(const char *string, FT_Error error)
{
	return SDL_Log("%s: %s", string, ErrorString(error));
}

static FT_Library library;

static struct FreeType
{
	FreeType()
	{
		FT_Error error = FT_Init_FreeType(&library);
		if (error) FT_perror("FT_InitFreeType", error);
	}	

	~FreeType()
	{
		FT_Error error = FT_Done_FreeType(library);
		if (error) FT_perror("FT_Done_FreeType", error);
	}
} FT;

bool FontStruct::Load(const char *path)
{
	FT_Error error = FT_New_Face(library, path, 0, &face);
	if (error) FT_perror("FT_New_Face", error);
	return !error;
}

void FontStruct::Free()
{
	FT_Error error = FT_Done_Face(face);
	if (error) FT_perror("FT_Done_Face", error);
}


