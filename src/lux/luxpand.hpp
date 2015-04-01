// This file uses sigma
// It's purpose is to generate lxthunk.hpp using gcc

// Template Meta-Functions

#define _template(n) ,  class ARG##n
#define _typename(n) ,        ARG##n
#define _argument(n) , lux_to<ARG##n>(vm,n+1)
#define _function(n)                                                 \
template <E(_template, n)>                                           \
 static int lux_thunk(lua_State *vm, void fn(E(_typename,n)))        \
 {                                                                   \
  fn(E(_argument, n));                                               \
  return 0;                                                          \
 }                                                                   \
template <E(_template, n), class RET>                                \
 static int lux_thunk(lua_State *vm, RET fn(E(_typename, n)))        \
 {                                                                   \
  lux_push(vm, fn(E(_argument, n)));                                 \
  return 1;                                                          \
 }                                                                   \
template <E(_template, n), class C>                                  \
 static int lux_thunk(lua_State *vm, void (C::*fn)(E(_typename, n))) \
 {                                                                   \
  C *obj = lux_to<C*>(vm, 1);                                        \
  lua_remove(vm, 1);                                                 \
  (obj->*fn)(E(_argument, n));                                       \
  return 0;                                                          \
 }                                                                   \
template <E(_template, n), class RET, class C>                       \
 static int lux_thunk(lua_State *vm, RET (C::*fn)(E(_typename, n)))  \
 {                                                                   \
  C *obj = lux_to<C*>(vm, 1);                                        \
  lua_remove(vm, 1);                                                 \
  lux_push(vm, (obj->*fn)(E(_argument, n)));                         \
  return 1;                                                          \
 }

g(_function, 9) // Expand Meta-Functions (lots of code here)

#undef _function
#undef _argument
#undef _typename
#undef _template

// Sigma does not resolve void argument variety; we must be explicit

static int lux_thunk(lua_State *vm, void fn(void))
{
	fn();
	return 0;
}

template <class RET> 
 static int lux_thunk(lua_State *vm, RET fn(void))
{
	lux_push(vm, fn());
	return 1;
}

template <class C>
 static int lux_thunk( lua_State *vm, void (C::*fn)(void))
{
	C *obj = lux_Class<C>::check(vm, 1);
	(obj->*fn)();
	return 0;
}

template <class RET, class C>
 static int lux_thunk(lua_State *vm, RET (C::*fn)(void))
{
	C *obj = lux_Class<C>::check(vm, 1);
	lux_push(vm, (obj->*fn)());
	return 1;
}

