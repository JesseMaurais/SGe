#ifndef __Font__
#define __Font__

#include "FreeType.hpp"
#include "Stroke.hpp"

struct CommonFont : FontStruct
{
	GLuint list, items;

	virtual void LoadChar(int code)=0;
	virtual bool LoadChars(int size, int items=128, int *list=NULL);
	virtual void FreeChars(void);

	void Draw(int x, int y, const char *string, int length=0);
	void Draw(int x, int y, const short *string, int length=0);
	void Draw(int x, int y, const long *string, int length=0);
};

struct TextureFont : CommonFont
{
	void LoadChar(int code);
	bool LoadChars(int size, int items=128, int *list=NULL);
	void FreeChars(void);

  protected:

	GLuint *text;

  private:

	static void Quad(FT_BitmapGlyph glyph);
	static void Texture(FT_BitmapGlyph glyph);
};

struct PolygonFont : CommonFont, Stroke
{
	void LoadChar(int code);

  private:

	typedef const FT_Vector *Point;
	static int Move(Point a, void *pen);
	static int Line(Point a, void *pen);
	static int Cone(Point a, Point b, void *pen);
	static int Cube(Point a, Point b, Point c, void *pen);
};

struct OutlineFont : PolygonFont
{
	OutlineFont(void);
};

#endif // file

