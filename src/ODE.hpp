#ifndef __ODE__
#define __ODE__

#define dDOUBLE
#include <ode/ode.h>

extern dWorldID World;
extern dSpaceID Space;

signed ODE_Init();
void ODE_Quit();
signed ODE_Wait();
signed ODE_Lock();
signed ODE_Unlock();

struct Geom
{
	virtual void Update()=0;
};

void Update(dGeomID);
void Update(dSpaceID);

#endif // file

