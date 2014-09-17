#ifndef __Hedron__
#define __Hedron__

#include "Number.hpp"

namespace Icosahedron
{
	const Scalar U = 0.5, V = Phi*U;

	const int nRects = 3;

	const int nPoints = nRects*4;

	static Scalar Rects [nRects][4][3] =
	{
	 { { 0,+V,+U },{ 0,-V,+U },{ 0,-V,-U },{ 0,+V,-U } }, // X
	 { { +U,0,+V },{ +U,0,-V },{ -U,0,-V },{ -U,0,+V } }, // Y
	 { { +V,+U,0 },{ -V,+U,0 },{ -V,-U,0 },{ +V,-U,0 } }  // Z
	};

	const int nFaces = 20;

	static struct { int box, point; } Faces [nFaces][3] =
	{
	 { {0,0},{1,3},{1,0} },
	 { {0,1},{1,0},{1,3} },
	 { {2,3},{1,0},{0,1} },
	 { {0,2},{2,3},{0,1} },

	 { {0,0},{1,0},{2,0} },
	 { {2,3},{2,0},{1,0} },
	 { {1,1},{2,0},{2,3} },
	 { {0,2},{1,1},{2,3} },

	 { {0,0},{2,0},{0,3} },
	 { {1,1},{0,3},{2,0} },
	 { {1,2},{0,3},{1,1} },
	 { {0,2},{1,2},{1,1} },

	 { {0,0},{0,3},{2,1} },
	 { {1,2},{2,1},{0,3} },
	 { {2,2},{2,1},{1,2} },
	 { {0,2},{2,2},{1,2} },

	 { {0,0},{2,1},{1,3} },
	 { {2,2},{1,3},{2,1} },
	 { {0,1},{1,3},{2,2} },
	 { {0,2},{0,1},{2,2} }
	};
		
	// Bitfield of faces in front of and crossing planes
	const int X = 0xFFF00, x = 0xC0300;
	const int Y = 0x8EEEC, y = 0x06024;
	const int Z = 0xFC88F, z = 0x10881;
	
	static void Draw(bool withRectangles=true);
};

#endif // file

