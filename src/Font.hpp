#ifndef Font_hpp
#define Font_hpp

#include "FreeType.hpp"
#include "Stroke.hpp"

struct Font : virtual FontStruct, Stroke
{
	void LoadChar(int code);
	void LoadChars(int items=128, int *list=nullptr);

  private:

	typedef const FT_Vector *Point;

	static int move(Point a, void *pen);
	static int line(Point a, void *pen);
	static int cone(Point a, Point b, void *pen);
	static int cube(Point a, Point b, Point c, void *pen);
};

#endif // file

