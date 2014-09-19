#ifndef __Crayon__
#define __Crayon__

#include "Vector.hpp"

struct Crayon
{
	enum Cap { CapButt, CapRound, CapSquare };
	enum Join { JoinBevel, JoinMiter, JoinRound, JoinConic };

	Crayon();
	void JoinStyle(enum Join style);
	void CapStyle(enum Cap style);
	void Width(Scalar thickness);
	void Vertex(Vector point);
	void Begin();
	void End();

 protected:

	Vector lineA, perpA;
	Vector lineB, perpB;

	Vector pivot;
	Scalar width;
	signed step;

 private:

	static Vector Perpendicular(Vector N);

	// Line

	void DrawSegment(Vector point);

	// Join

	enum Join join;

	void DrawJoin();
	void DrawJoinBevel(Vector U, Vector V);
	void DrawJoinMiter(Vector U, Vector V, double d);
	void DrawJoinRoundPart(Vector U, Vector V, int n);
	void DrawJoinRound(Vector U, Vector V);
	void DrawJoinConic(Vector U, Vector V, double d);

	// Cap

	enum Cap cap;

	void DrawCap();
	void DrawCapSquare();
	void DrawCapRound();
};

#endif // file

