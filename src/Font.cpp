#include "Font.hpp"

void Font::LoadChars(int items, int *list)
{
	for (int it = 0; it < items; ++it)
	{
	 LoadChar(list ? list[it] : it);
	}
}

void Font::LoadChar(int code)
{
	FT_Load_Char(face, code, FT_LOAD_NO_BITMAP);
	FT_Outline_Funcs funcs =
	{
	 move, line, cone, cube, 0, 0
	};
	BeginGroup(code);
	BeginPolygon();
	FT_Outline_Decompose(&face->glyph->outline, &funcs, this);
	EndPolygon();
	EndGroup();
}

int Font::move(const FT_Vector *a, void *pen)
{
	Vector A(a->x, a->y);
	static_cast<Stroke*>(pen)->Move(A);
	return 0;
}

int Font::line(const FT_Vector *a, void *pen)
{
	Vector A(a->x, a->y);
	static_cast<Stroke*>(pen)->Line(A);
	return 0;
}

int Font::cone(const FT_Vector *a, const FT_Vector *b, void *pen)
{
	Vector A(a->x, a->y);
	Vector B(b->x, b->y);
	static_cast<Stroke*>(pen)->Cone(A, B);
	return 0;
}

int Font::cube(const FT_Vector *a, const FT_Vector *b, const FT_Vector *c, void *pen)
{
	Vector A(a->x, a->y);
	Vector B(b->x, b->y);
	Vector C(c->x, c->y);
	static_cast<Stroke*>(pen)->Cube(A, B, C);
	return 0;
}

