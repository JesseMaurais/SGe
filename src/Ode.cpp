#include "Ode.hpp"
#include "SDL.hpp"
#include "Lua.hpp"
#include "Event.hpp"
#include <cstdlib>
#include <cstdio>


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
	 //SDL::LogError("SDL_SetThreadPriority");
	}
	while (!Quit)
	{
		if (SDL_LockMutex(Mutex))
		{
			SDL::LogError("SDL_LockMutex");
			break;
		}
		if (SDL_CondWait(Cond, Mutex))
		{
			SDL::LogError("SDL_CondWait");
			break;
		}
		if (not SDL::SendUserEvent(UpdateSpace))
		{
			SDL::LogError("UpdateSpace");
			break;
		}
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
		SDL::LogError("SDL_LockMutex");
	}
	else
	if (SDL_CondSignal(Cond))
	{
		SDL::LogError("SDL_CondSignal");
	}
	else
	{
		SDL_UnlockMutex(Mutex);
	}

	double elapsed = interval;
	double approx = elapsed / 1000;
	double error = Step - approx;
	double step = Step + error;

	return Uint32(1000*step);
}

static void Error(int error, const char *format, va_list va)
{
	static char string[BUFSIZ];
	std::vsnprintf(string, BUFSIZ, format, va);
	SDL_Log("ODE: %s", string);
	std::exit(EXIT_FAILURE);
}

signed Ode_Init()
{
	Quit = SDL_FALSE;
	dInitODE2(dInitFlagManualThreadCleanup);
	dSetMessageHandler(Error);
	dSetDebugHandler(Error);
	dSetErrorHandler(Error);

	World = dWorldCreate();
	Space = dHashSpaceCreate(0);
	Group = dJointGroupCreate(0);
	Step = 1.0/50.0;

	lua_getglobal(State, "World");
	int table = lua_gettop(State);

	if (!lua_isnil(State, table))
	{
	 lua_pushnil(State);
	 while (lua_next(State, table))
	 {
		const char *key = lua_tostring(State, -2);
		#define tointeger lua_tointeger(State, -1)
		#define toboolean lua_toboolean(State, -1)
		#define tonumber  lua_tonumber(State, -1)

		if (!SDL_strcasecmp(key, "FPS"))
		{
		 Step = 1.0/tonumber;
		}
		else
		if (!SDL_strcasecmp(key, "ERP"))
		{
		 dWorldSetERP(World, tonumber);
		}
		else
		if (!SDL_strcasecmp(key, "CFM"))
		{
		 dWorldSetCFM(World, tonumber);
		}
		else
		if (!SDL_strcasecmp(key, "LINEAR_DAMPING"))
		{
		 dWorldSetLinearDamping(World, tonumber);
		}
		else
		if (!SDL_strcasecmp(key, "LINEAR_DAMPING_THRESHOLD"))
		{
		 dWorldSetLinearDampingThreshold(World, tonumber);
		}
		else
		if (!SDL_strcasecmp(key, "ANGULAR_DAMPING"))
		{
		 dWorldSetAngularDamping(World, tonumber);
		}
		else
		if (!SDL_strcasecmp(key, "ANGULAR_DAMPING_THRESHOLD"))
		{
		 dWorldSetAngularDampingThreshold(World, tonumber);
		}
		else
		if (!SDL_strcasecmp(key, "MAX_ANGULAR_SPEED"))
		{
		 dWorldSetMaxAngularSpeed(World, tonumber);
		}
		else
		if (!SDL_strcasecmp(key, "CONTACT_MAX_CORRECTING_VELOCITY"))
		{
		 dWorldSetContactMaxCorrectingVel(World, tonumber);
		}
		else
		if (!SDL_strcasecmp(key, "CONTACT_SURFACE_LAYER"))
		{
		 dWorldSetContactSurfaceLayer(World, tonumber);
		}
		else
		if (!SDL_strcasecmp(key, "AUTO_DISABLE"))
		{
		 dWorldSetAutoDisableFlag(World, toboolean);
		}
		else
		if (!SDL_strcasecmp(key, "AUTO_DISABLE_LINEAR_THRESHOLD"))
		{
		 dWorldSetAutoDisableLinearThreshold(World, tonumber);
		}
		else
		if (!SDL_strcasecmp(key, "AUTO_DISABLE_ANGULAR_THRESHOLD"))
		{
		 dWorldSetAutoDisableAngularThreshold(World, tonumber);
		}
		else
		if (!SDL_strcasecmp(key, "AUTO_DISABLE_STEPS"))
		{
		 dWorldSetAutoDisableSteps(World, tointeger);
		}
		else
		if (!SDL_strcasecmp(key, "AUTO_DISABLE_TIME"))
		{
		 dWorldSetAutoDisableTime(World, tonumber);
		}
		else
		{
		 SDL_Log("World: %s does not match", key);
		}
		lua_pop(State, 1);
	 }
	}
	lua_pop(State, 1);

	Cond = SDL_CreateCond();
	if (!Cond)
	{
	 dWorldDestroy(World);
	 dSpaceDestroy(Space);
	 dJointGroupDestroy(Group);
	 SDL::LogError("SDL_CreateCond");
	 return SDL_SetError("cannot create simulation signal");
	}
	Mutex = SDL_CreateMutex();
	if (!Mutex)
	{
	 dWorldDestroy(World);
	 dSpaceDestroy(Space);
	 dJointGroupDestroy(Group);
	 SDL_DestroyCond(Cond);
	 SDL::LogError("SDL_CreateMutex");
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
	 SDL::LogError("SDL_CreateThread");
	 return SDL_SetError("cannot create simulation thread");
	}
	TimerID = SDL_AddTimer(Uint32(1000*Step), SimulationTimer, NULL);
	if (!TimerID)
	{
	 dWorldDestroy(World);
	 dSpaceDestroy(Space);
	 dJointGroupDestroy(Group);
	 SDL_DestroyCond(Cond);
	 SDL_DestroyMutex(Mutex);
	 SDL::LogError("SDL_AddTimer");
	 return SDL_SetError("cannot create simulation timer");
	}
	return 0;
}

void Ode_Quit()
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

signed Ode::Lock()
{
	if (SDL_LockMutex(Mutex))
	{
		SDL::LogError("SDL_LockMutex");
		return -1;
	}
	return 0;
}

signed Ode::Unlock()
{
	if (SDL_UnlockMutex(Mutex))
	{
		SDL::LogError("SDL_UnlockMutex");
		return -1;
	}
	return 0;
}

void Ode::Update(dGeomID geom)
{
	union {
	 void *data;
	 Geom *obj;
	};
	data = dGeomGetData(geom);
	if (obj) obj->UpdateSource();
	if (dGeomIsSpace(geom))
	{
		Ode::Update(dSpaceID(geom));
	}
}

void Ode::Update(dSpaceID space)
{
	int n = dSpaceGetNumGeoms(space);
	for (int item = 0; item < n; ++item)
	{
		Ode::Update(dSpaceGetGeom(space, item));
	}
}

