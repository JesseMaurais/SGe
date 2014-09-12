#include "ODE.hpp"
#include "SDL.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

dWorldID World;
dSpaceID Space;
static dJointGroupID Group;
static dReal Step;
static SDL_TimerID TimerID;
static SDL_Thread *Thread;
static SDL_mutex *Mutex;
static SDL_cond *Cond;
static SDL_bool Quit;

static void Near(void *unused, dGeomID g1, dGeomID g2)
{
	bool s1 = dGeomIsSpace(g1);
	bool s2 = dGeomIsSpace(g2);

	if (s1 || s2)
	{
		dSpaceCollide2(g1, g2, unused, &Near);
		if (s1)
		 dSpaceCollide(dSpaceID(g1), unused, &Near);
		if (s2)
		 dSpaceCollide(dSpaceID(g2), unused, &Near);
	}
	else
	{
		dBodyID b1 = dGeomGetBody(g1);
		dBodyID b2 = dGeomGetBody(g2);

		const int N = 42;
		dContact contact[N];
		for (int i = 0; i < N; ++i)
		{
		 contact[i].surface.mode       = dContactBounce|dContactSoftCFM;
		 contact[i].surface.mu         = dInfinity;
		 contact[i].surface.mu2        = 0;
		 contact[i].surface.bounce     = 0.1;
		 contact[i].surface.bounce_vel = 0.1;
		 contact[i].surface.soft_cfm   = 0.01;
		}

		int n = dCollide(g1, g2, N, &contact->geom, sizeof(dContact));
		for (int i = 0; i < n; ++i)
		{
		 dJointID joint = dJointCreateContact(World, Group, contact+i);
		 dJointAttach(joint, b1, b2);
		}
	}
}

static int SimulationThread(void *unused)
{
	dAllocateODEDataForThread(dAllocateFlagCollisionData);
	if (SDL_SetThreadPriority(SDL_THREAD_PRIORITY_HIGH))
	{
	 //SDL_perror("SDL_SetThreadPriority");
	}
	while (!Quit)
	{
		if (SDL_LockMutex(Mutex))
		{
		 SDL_perror("SDL_LockMutex");
		 break;
		}
		if (SDL_CondWait(Cond, Mutex))
		{
		 SDL_perror("SDL_CondWait");
		 break;
		}
		PushEvent(UPDATE);
		dSpaceCollide(Space, 0, &Near);
		dWorldStep(World, Step);
		dJointGroupEmpty(Group);
		SDL_UnlockMutex(Mutex);
	}
	dCleanupODEAllDataForThread();
	return 0;
}

static Uint32 SimulationTimer(Uint32 interval, void *unused)
{
	if (SDL_LockMutex(Mutex))
	{
	 SDL_perror("SDL_LockMutex");
	}
	else
	if (SDL_CondBroadcast(Cond))
	{
	 SDL_perror("SDL_CondBroadcast");
	}
	else
	{
	 SDL_UnlockMutex(Mutex);
	}
	return interval;	
}

static void Error(int error, const char *format, va_list va)
{
	static char string[BUFSIZ];
	vsnprintf(string, BUFSIZ, format, va);
	SDL_Log("ODE: %s", string);
	exit(EXIT_FAILURE);
}

signed ODE_Init()
{
	Quit = SDL_FALSE;
	dInitODE2(dInitFlagManualThreadCleanup);
	dSetMessageHandler(Error);
	dSetDebugHandler(Error);
	dSetErrorHandler(Error);

	World = dWorldCreate();
	Space = dHashSpaceCreate(0);
	Group = dJointGroupCreate(0);
	Step = 50.0/1000.0;

	Cond = SDL_CreateCond();
	if (!Cond)
	{
	 dWorldDestroy(World);
	 dSpaceDestroy(Space);
	 dJointGroupDestroy(Group);
	 SDL_perror("SDL_CreateCond");
	 return SDL_SetError("cannot create simulation signal");
	}
	Mutex = SDL_CreateMutex();
	if (!Mutex)
	{
	 dWorldDestroy(World);
	 dSpaceDestroy(Space);
	 dJointGroupDestroy(Group);
	 SDL_DestroyCond(Cond);
	 SDL_perror("SDL_CreateMutex");
	 return SDL_SetError("cannot create simulation mutex");
	}
	Thread = SDL_CreateThread(SimulationThread, "ODE", NULL);
	if (!Thread)
	{
	 dWorldDestroy(World);
	 dSpaceDestroy(Space);
	 dJointGroupDestroy(Group);
	 SDL_DestroyCond(Cond);
	 SDL_DestroyMutex(Mutex);
	 SDL_perror("SDL_CreateThread");
	 return SDL_SetError("cannot create simulation thread");
	}
	TimerID = SDL_AddTimer(1000/50, SimulationTimer, NULL);
	if (!TimerID)
	{
	 dWorldDestroy(World);
	 dSpaceDestroy(Space);
	 dJointGroupDestroy(Group);
	 SDL_DestroyCond(Cond);
	 SDL_DestroyMutex(Mutex);
	 SDL_perror("SDL_AddTimer");
	 return SDL_SetError("cannot create simulation timer");
	}
	return 0;
}

void ODE_Quit()
{
	Quit = SDL_TRUE;
	SDL_WaitThread(Thread, NULL);
	SDL_RemoveTimer(TimerID);
	SDL_DestroyMutex(Mutex);
	SDL_DestroyCond(Cond);
	dWorldDestroy(World);
	dSpaceDestroy(Space);
	dJointGroupDestroy(Group);
	dCloseODE();
}

signed ODE_Lock()
{
	if (SDL_LockMutex(Mutex))
	{
	 SDL_perror("SDL_LockMutex");
	 return -1;
	}
	return 0;
}

signed ODE_Wait()
{
	if (SDL_LockMutex(Mutex))
	{
	 SDL_perror("SDL_LockMutex");
	 return -1;
	}
	if (SDL_CondWait(Cond, Mutex))
	{
	 SDL_perror("SDL_CondWait");
	 return -1;
	}
	return 0;
}

signed ODE_Unlock()
{
	if (SDL_UnlockMutex(Mutex))
	{
	 SDL_perror("SDL_UnlockMutex");
	 return -1;
	}
	return 0;
}

void Update(dGeomID geom)
{
	union {
	 void *data;
	 Geom *obj;
	};
	data = dGeomGetData(geom);
	if (obj) obj->Update();

	if (dGeomIsSpace(geom))
	{
	 Update(dSpaceID(geom));
	}
}

void Update(dSpaceID space)
{
	int n = dSpaceGetNumGeoms(space);
	for (int item = 0; item < n; ++item)
	{
	 Update(dSpaceGetGeom(space, item));
	}
}

