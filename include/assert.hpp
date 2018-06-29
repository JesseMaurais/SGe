#ifndef assert_hpp
#define assert_hpp

#include <cassert>

#ifdef NDEBUG
#define verify(condition) (condition)
#else
#define verify(condition) assert(condition)
#endif

#endif // file

