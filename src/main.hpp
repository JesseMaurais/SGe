#ifndef main_hpp
#define main_hpp

// PRECOMPILED HEADER

// C Standard

#include <ciso646>
#include <cstring>
#include <cinttypes>
#include <climits>
#include <cfloat>
#include <cfenv>
#include <cstdlib>
#include <cstdio>
#include <cmath>

// C++ Containers

#include <string>
#include <vector>
#include <array>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <list>

// C++ Numerics

#include <algorithm>
#include <numeric>
#include <complex>
#include <random>

// C++ Streams

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

// OpenGL

#include <GL/glew.h>
#include <GL/glu.h>

// OpenAL

#include <AL/alc.h>
#include <AL/al.h>

// OpenCL

#include <CL/cl.h>

// Ode

#define dDOUBLE
#include <ode/ode.h>

// Lua

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

// SDL2

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>


#endif // file
