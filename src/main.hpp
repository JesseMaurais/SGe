#ifndef main_hpp
#define main_hpp

// PRECOMPILED HEADER

// Standard C

#include <cassert>
#include <cerrno>
#include <ciso646>
#include <cstring>
#include <cstdint>
#include <cinttypes>
#include <climits>
#include <cfloat>
#include <cfenv>
#include <cstdlib>
#include <cstdio>
#include <cmath>

// C++ Containers

//#include <string_view>
#include <string>
#include <vector>
#include <array>
#include <stack>
#include <queue>
#include <deque>
#include <set>
#include <map>
#include <list>
//#include <unordered_set>
//#include <unordered_map>
//#include <forward_list>
#include <iterator>

// C++ Numerics

#include <algorithm>
#include <numeric>
#include <complex>
#include <random>
#include <ratio>
#include <limits>

// C++ Streams

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

// C++ Threads

#include <atomic>
#include <thread>
#include <future>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>

// C++ Utility

#include <exception>
#include <stdexcept>
#include <system_error>
#include <utility>
#include <bitset>
#include <tuple>
#include <chrono>
#include <functional>
#include <initializer_list>
#include <memory>
//#include <memory_resource>
//#include <scoped_allocator>
#include <new>

// C++ Runtime Type Information

#include <typeinfo>
#include <typeindex>
#include <type_traits>

// JavaScript

#include <jerryscript.h>

// XML

#include <expat.h>

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

// SDL2

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// Make all asserts use the SDL assert window
#undef assert
#define assert(condition) SDL_assert(condition)


#endif // file
