#include "Font.hpp"
#include <cstring>

// Common /////////////////////////////////////////////////////////////////////

bool CommonFont::LoadChars(int size, int items, int *list)
{
	FT_Error error = FT_Set_Pixel_Sizes(face, 0, size);
	if (error) return false;
	this->items = items;
	this->list = glGenLists(items);
	for (int it = 0; it < items; ++it)
	{
	 LoadChar(list ? list[it] : it);
	}
	return true;
}

void CommonFont::FreeChars(void)
{
	glDeleteLists(list, items);
}

void CommonFont::Draw(int x, int y, const char *string, int length)
{
	if (!length) length = strlen(string);
	glListBase(list);
	glTranslated(x, y, 0);
	glCallLists(length, GL_UNSIGNED_BYTE, string);
}

void CommonFont::Draw(int x, int y, const short *string, int length)
{
	if (!length) while (!string[++length]);
	glListBase(list);
	glTranslated(x, y, 0);
	glCallLists(length, GL_UNSIGNED_SHORT, string);
}

void CommonFont::Draw(int x, int y, const long *string, int length)
{
	if (!length) while (!string[++length]);
	glListBase(list);
	glTranslated(x, y, 0);
	glCallLists(length, GL_UNSIGNED_INT, string);
}

// Texture ////////////////////////////////////////////////////////////////////

bool TextureFont::LoadChars(int size, int items, int *list)
{
	text = new GLuint[items];
	glGenTextures(items, text);
	if (!CommonFont::LoadChars(size, items, list))
	{
	 glDeleteTextures(items, text);
	 delete text;
	 return false;
	}
	return true;
}

void TextureFont::FreeChars(void)
{
	CommonFont::FreeChars();
	glDeleteTextures(items, text);
	delete text;
}

void TextureFont::LoadChar(int code)
{
	FT_Glyph glyph;
	
	FT_Load_Char(face, code, FT_LOAD_DEFAULT);
	FT_Get_Glyph(face->glyph, &glyph);
	FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
	
	glBindTexture(GL_TEXTURE_2D, text[code]);
	Texture(FT_BitmapGlyph(glyph));
	glNewList(list+code, GL_COMPILE);
	glPushName(code);
	glBindTexture(GL_TEXTURE_2D, text[code]);
	Quad(FT_BitmapGlyph(glyph));
	glTranslated(face->glyph->advance.x >> 6, 0, 0);
	glPopName();
	glEndList();
	
	FT_Done_Glyph(glyph);
}

static unsigned pow2(double i)
{
	unsigned j = 1;
	while (j < i) j <<= 1;
	return j;
}

void TextureFont::Quad(FT_BitmapGlyph glyph)
{
	double s,t,u,v;
	
	u = glyph->bitmap.width;
	v = glyph->bitmap.rows;
	
	s = u / ::pow2(u);
	t = v / ::pow2(v);
	
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2d(0, v);
	glTexCoord2d(0, t); glVertex2d(0, 0);
	glTexCoord2d(s, t); glVertex2d(u, 0);
	glTexCoord2d(s, 0); glVertex2d(u, v);
	glEnd();
}

void TextureFont::Texture(FT_BitmapGlyph glyph)
{
	int u, v, w, x, y, z;
	
	u = ::pow2(glyph->bitmap.width);
	v = ::pow2(glyph->bitmap.rows);
	
	char buffer[u*v*2];
	memset(buffer, 0, sizeof(buffer));
	for (w = y = 0; y < glyph->bitmap.rows; ++y)
	 for (x = 0; x < glyph->bitmap.width; ++x, ++w)
	  for (z = 0; z < 2; ++z)
	   buffer[(y * u + x) * 2 + z] = glyph->bitmap.buffer[w];
	
	glTexParameteri
	(GL_TEXTURE_2D
	,GL_TEXTURE_MIN_FILTER
	,GL_LINEAR_MIPMAP_LINEAR
	);
	glTexParameteri
	(GL_TEXTURE_2D
	,GL_TEXTURE_MAG_FILTER
	,GL_LINEAR_MIPMAP_LINEAR
	);
	glTexParameteri
	(GL_TEXTURE_2D
	,GL_TEXTURE_WRAP_S
	,GL_CLAMP_TO_EDGE
	);
	glTexParameteri
	(GL_TEXTURE_2D
	,GL_TEXTURE_WRAP_T
	,GL_CLAMP_TO_EDGE
	);
	gluBuild2DMipmaps
	(GL_TEXTURE_2D
	,GL_RGBA
	,u
	,v
	,GL_LUMINANCE_ALPHA
	,GL_UNSIGNED_BYTE
	,buffer
	);
}

// Polygon ////////////////////////////////////////////////////////////////////

void PolygonFont::LoadChar(int code)
{
	FT_Load_Char(this->face, code, FT_LOAD_NO_BITMAP);
	
	static FT_Outline_Funcs funcs =
	{
	 Move, Line, Cone, Cube, 0, 0
	};
					
	glNewList(this->list+code, GL_COMPILE);
	glPushName(code);
	glPushMatrix();
	const double inv = 1.0/64.0;
	glScaled(inv, inv, 1);
	Begin();
	FT_Outline_Decompose(&this->face->glyph->outline, &funcs, this);
	End();
	glPopMatrix();
	glTranslated(this->face->glyph->advance.x >> 6, 0, 0);
	glPopName();
	glEndList();
}

int PolygonFont::Move(Point a, void *pen)
{
	Vector A(a->x, a->y);
	static_cast<Stroke*>(pen)->Move(A);
	return 0;
}

int PolygonFont::Line(Point a, void *pen)
{
	Vector A(a->x, a->y);
	static_cast<Stroke*>(pen)->Line(A);
	return 0;
}

int PolygonFont::Cone(Point a, Point b, void *pen)
{
	Vector A(a->x, a->y);
	Vector B(b->x, b->y);
	static_cast<Stroke*>(pen)->Cone(A, B);
	return 0;
}

int PolygonFont::Cube(Point a, Point b, Point c, void *pen)
{
	Vector A(a->x, a->y);
	Vector B(b->x, b->y);
	Vector C(c->x, c->y);
	static_cast<Stroke*>(pen)->Cube(A, B, C);
	return 0;
}

// Outline ////////////////////////////////////////////////////////////////////

OutlineFont::OutlineFont(void)
{
	Stroke::Property(GLU_TESS_BOUNDARY_ONLY);
}

