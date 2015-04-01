
#ifndef __lxsigma__
#define __lxsigma__

// Macro-Expansion Magic Spell

#define f0(g)       g(0)
#define f1(g) f0(g) g(1)
#define f2(g) f1(g) g(2)
#define f3(g) f2(g) g(3)
#define f4(g) f3(g) g(4)
#define f5(g) f4(g) g(5)
#define f6(g) f5(g) g(6)
#define f7(g) f6(g) g(7)
#define f8(g) f7(g) g(8)
#define f9(g) f8(g) g(9)

#define g0(f)       f(0)
#define g1(f) g0(f) f(1)
#define g2(f) g1(f) f(2)
#define g3(f) g2(f) f(3)
#define g4(f) g3(f) f(4)
#define g5(f) g4(f) f(5)
#define g6(f) g5(f) f(6)
#define g7(f) g6(f) f(7)
#define g8(f) g7(f) f(8)
#define g9(f) g8(f) f(9)

#define f(g,n) f##n(g)
#define g(f,n) g##n(f)

// This forces the parser to eat a comma

#define x(...) __VA_ARGS__
#define h(n,...) __VA_ARGS__
#define e(n) x(h(0 n))
#define E(g,n) e(f(g,n))

#endif // file

