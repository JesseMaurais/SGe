#ifndef main_hpp
#define main_hpp

// PRECOMPILED HEADER

// Standard C

#include <cassert>
#include <cerrno>
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
#include <iterator>

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

// RTTI

#include <typeinfo>
#include <type_traits>

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

#include <lua5.3/lua.hpp>

// SDL2

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>

// Make all asserts use the SDL assert window
#undef assert
#define assert(condition) SDL_assert(condition)


#endif // file
