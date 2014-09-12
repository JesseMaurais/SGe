#ifndef __Model__
#define __Model__

#include "Mesh.hpp"
#include "Nurbs.hpp"
#include "Tessel.hpp"

struct Model : virtual Nurbs, Tessel
{
	void Begin(int mode);
	void Next(int index);
	void End();

	void Vertex(double x, double y, double z);
	void TexCoord(double x, double y, double z);
	void Normal(double x, double y, double z);
	
 private:

	bool flag;
};

#endif // file

