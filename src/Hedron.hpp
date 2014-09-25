#ifndef __Hedron__
#define __Hedron__

#include "Number.hpp"

namespace icos
{
	const int nRects = 3;
	extern Scalar Rects [nRects][4][3];

	struct Point { int box, point; };

	const int nPoints = 12;
	extern Point Points [nPoints];

	struct Edge { int points[2]; };

	const int nEdges = 30;
	extern Edge Edges[nEdges];

	struct Face { int edges[3]; };

	const int nFaces = 20;
	extern Point RFaces[nFaces][3];
	extern Face EFaces[nFaces];
	extern int PFaces[nFaces][3];
};

#endif // file

