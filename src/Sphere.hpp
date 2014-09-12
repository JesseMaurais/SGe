#ifndef __Sphere__
#define __Sphere__

#include "Hedron.hpp"
#include "Vector.hpp"

struct Sphere
{
	static void Vertex(Vector &V, double s, double t);
	static void Draw(Vector V [3]);
	static void Divide(unsigned depth, Vector V [3]);
	static void Draw(unsigned depth);
};

#endif // file


/* NOTES
 *
 * At depth = 0 the texture on the polar triangle in the negative x is skewed.
 * This artifact disappears where depth > 0
 */

