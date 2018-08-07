#ifndef dbg_hpp
#define dbg_hpp

#include <cassert>

#ifndef NDEBUG
#define verify(x) (x)
constexpr bool DEBUG = false;
#else
#define verify(x) assert(x)
constexpr bool DEBUG = true;
#endif

#endif // file
