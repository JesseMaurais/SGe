#ifndef Ode_hpp
#define Ode_hpp

#define dDOUBLE
#include <ode/ode.h>
#include "Source.hpp"

namespace Ode
{
	void Update(dGeomID);
	void Update(dSpaceID);

	signed Lock();
	signed Unlock();
}

struct Geom : Source
{

};

#endif // file
