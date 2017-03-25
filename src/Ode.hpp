#ifndef Ode_hpp
#define Ode_hpp

#define dDOUBLE
#include <ode/ode.h>

extern dWorldID World;
extern dSpaceID Space;

signed Ode_Init();
void Ode_Quit();
signed Ode_Lock();
signed Ode_Unlock();

struct Geom
{
	virtual void Update() = 0;
	virtual ~Geom() = default;
};

void Update(dGeomID);
void Update(dSpaceID);

#endif // file
