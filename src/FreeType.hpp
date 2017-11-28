#ifndef FreeType_hpp
#define FreeType_hpp

#include <ft2build.h>
#include <freetype2/freetype/ftglyph.h>
#include <freetype2/freetype/ftoutln.h>

namespace FT
{
	void SetError(FT_Error const error);
}

struct CharStruct
{
	FT_Pos ascent, descent, advance;
};

struct FontStruct
{
	FT_Face face;

	bool Load(const char *path);
	void Free();

	template <class Char> 
	CharStruct Measure(const Char *string, int length=0)
	{
		CharStruct extent = {0, 0, 0};
		if (!length) while (string[++length]);
		for (int it = 0; it < length; ++it)
		{
			FT_Load_Char(face, string[it], FT_LOAD_NO_BITMAP);
			FT_Glyph_Metrics &meter = face->glyph->metrics;

			extent.advance += meter.horiAdvance;
			if (extent.ascent < meter.horiBearingY)
				extent.ascent = meter.horiBearingY;

			FT_Pos diff = meter.height - meter.horiBearingY;
			if (extent.descent < diff)
				extent.descent = diff;
		}
		return extent;
	}
	
	template <class Char> 
	CharStruct Measure(Char **list, int items=0)
	{
		CharStruct extent = {0, 0, 0};
		if (!items) while (list[++items]);
		for (int it = 0; it < items; ++it)
		{
			CharStruct string = Measure(list[it]);

			extent.ascent += string.ascent;
			extent.descent += string.descent;
			if (extent.advance < string.advance)
				extent.advance = string.advance;
		}
		return extent;
	}

	template <class Char> 
	void Measure(CharStruct *extents, const Char *string, int length=0)
	{
		if (!length) while (string[++length]);
		for (int it = 0; it < length; ++it)
			extents[it] = Measure(string+it, 1);
	}

	template <class Char> 
	void Measure(CharStruct *extents, Char **list, int items=0)
	{
		if (!items) while (list[++items]);
		for (int it = 0; it < items; ++it)
			extents[it] = Measure(list[it]);
	}
};

#endif // file

