#ifndef __Crayon__
#define __Crayon__

#include "Vector.hpp"

class Crayon
{
	Vector lineA, perpA;
	Vector lineB, perpB;

	Vector pivot;
	double width;
	signed step;

 public:

	enum Cap { CapButt, CapRound, CapSquare };
	enum Join { JoinBevel, JoinMiter, JoinRound, JoinConic };

	Crayon(void);

	void JoinStyle(enum Join style);
	void CapStyle(enum Cap style);
	void Width(double thickness);
	void Begin(void);
	void End(void);
	void Vertex(Vector point);

 private:

	static Vector Perpendicular(Vector N);

	// Line

	void DrawSegment(Vector point);

	// Join

	enum Join join;

	void DrawJoin(void);
	void DrawJoinBevel(Vector U, Vector V);
	void DrawJoinMiter(Vector U, Vector V, double d);
	void DrawJoinRoundPart(Vector U, Vector V, int n);
	void DrawJoinRound(Vector U, Vector V);
	void DrawJoinConic(Vector U, Vector V, double d);

	// Cap

	enum Cap cap;

	void DrawCap(void);
	void DrawCapSquare(void);
	void DrawCapRound(void);
};

#endif // file

