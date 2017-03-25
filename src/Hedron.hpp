#ifndef Hedron_hpp
#define Hedron_hpp

#include "Number.hpp"

namespace icos
{
	const int nRects = 3;
	extern Scalar Rects [nRects][4][3];

	const int nPoints = 12;
	struct Point { int box, point; };
	extern Point Points [nPoints];

	const int nEdges = 30;
	struct Edge { int points[2]; };
	extern Edge Edges[nEdges];

	const int nFaces = 20;
	struct Face { int edges[3]; };
	extern Point RFaces[nFaces][3];
	extern Face EFaces[nFaces];
	extern int PFaces[nFaces][3];
};

#endif // file

