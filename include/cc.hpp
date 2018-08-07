#ifndef cc_hpp
#define cc_hpp

// https://sourceforge.net/p/predef/wiki/Compilers/

namespace cc 
{

// GNU Compiler Collection
#if defined(__GNUC__)
constexpr bool GNU = true;
#else
constexpr bool GNU = false;
#endif

// Minimalist GNU for Windows
#if defined(__MINGW32__) || defined(__MINGW64__)
constexpr bool MINGW = true;
#else
constexpr bool MINGW = false;
#endif

// DJ's GNU Programming Platform
#if defined(__DJGPP__) || defined(__GO32__)
constexpr bool DJGPP = true;
#else
constexpr bool DJGPP = false;
#endif

// Clang
#if defined(__clang__)
constexpr bool CLANG = true;
#else
constexpr bool CLANG = false;
#endif

// LLVM
#if defined(__llvm__)
constexpr bool LLVM = true;
#else
constexpr bool LLVM = false;
#endif

// Digital Mars
#if defined(__DMC__)
constexpr bool DMC = true;
#else
constexpr bool DMC = false;
#endif

// Microsoft Visual C++
#if defined(_MSC_VER)
constexpr bool MSVC = true;
#else
constexpr bool MSVC = false;
#endif

// IBM
#if defined(__IBMC__) || defined(__IBMCPP__) || defined(__xlc__) || defined(__xlC__)
constexpr bool IBM = true;
#else
constexpr bool IBM = false;
#endif

// Intel
#if defined(__INTEL_COMPILER) || defined(__ICC) || defined(__ECC) || defined(__ICL)
constexpr bool INTEL = true;
#else
constexpr bool INTEL = false;
#endif

}

#endif // file
