#ifndef __lxthunk__
#define __lxthunk__

#include "lxalloc.hpp"
#include "lxstack.hpp"
#include "lxclass.hpp"

// Wrap a function pointer so that we can cast to void* safely

template <class F> struct lux_Function
{
	F fun;

	explicit lux_Function(F x) : fun(x) { }
};

// Store the function pointer in the upvalue, dispatch by signature

template <class F>
 static int lux_closure(lua_State *vm)
{
	union {
	 lux_Function<F> *obj;
	 void *addr;
	};

	int up = lua_upvalueindex(1);
	addr = lua_touserdata(vm, up);
	return lux_thunk(vm, obj->fun);
}

template <class F>
 static void lux_register(lua_State *vm, F fun, const char *name)
{
	new (vm) lux_Function<F>(fun);
	lua_pushcclosure(vm, lux_closure<F>, 1);
	lua_setglobal(vm, name);
}

template <class F>
 static void lux_register(lua_State *vm, int tab, F fun, const char *name)
{
	new (vm) lux_Function<F>(fun);
	lua_pushcclosure(vm, lux_closure<F>, 1);
	lua_setfield(vm, tab, name);
}

// These do an implicit string conversion of the function name

#define lux_function(vm, fun) lux_register(vm, fun, #fun)

#define lux_method(vm, tab, obj, fun) lux_register(vm, tab, &obj::fun, #fun)

// If you're using C++11 then you don't need to store pointers in up-values

#define lux_wrap(fun)\
 lua_CFunction([](lua_State *vm)->int{ return lux_thunk(vm, &fun); })

#define lux_reg(fun) {#fun, lux_wrap(fun)} // luaL_Reg compatible

/******************************************************************************
 *
 * The following are function prototypes of the form
 *
 *	template <class F> int lux_thunk(lua_State *vm, F fun);
 *
 * which call the function "fun" after converting the arguments appropriately
 * from the Lua stack and pushing the C++ return value onto the stack if "fun"
 * has a return value.
 */

// this one needs no template
 static int lux_thunk(lua_State *vm, void fun(void))
 {
  fun();
  return 0;
 }

template <class RET>
 static int lux_thunk(lua_State *vm, RET fun(void))
 {
  lux_push<RET>(vm, fun());
  return 1;
 }

template <class C>
 static int lux_thunk(lua_State *vm, void (C::*fun)(void))
 {
  C *obj = lux_to<C*>(vm, 1);
  (obj->*fun)();
  return 0;
 }

template <class RET, class C>
 static int lux_thunk(lua_State *vm, RET (C::*fun)(void))
 {
  C *obj = lux_to<C*>(vm, 1);
  lux_push<RET>(vm, (obj->*fun)());
  return 1;
 }

template <class ARG0>
 static int lux_thunk(lua_State *vm, void fun(ARG0))
 {
  fun(lux_to<ARG0>(vm,0 +1));
  return 0;
 }

template <class ARG0, class RET>
 static int lux_thunk(lua_State *vm, RET fun(ARG0))
 {
  lux_push<RET>(vm, fun(lux_to<ARG0>(vm,0 +1)));
  return 1;
 }

template <class ARG0, class C>
 static int lux_thunk(lua_State *vm, void (C::*fun)(ARG0))
 {
  C *obj = lux_to<C*>(vm, 1);
  lua_remove(vm, 1);
  (obj->*fun)(lux_to<ARG0>(vm,0 +1));
  return 0;
 }

template <class ARG0, class RET, class C>
 static int lux_thunk(lua_State *vm, RET (C::*fun)(ARG0))
 {
  C *obj = lux_to<C*>(vm, 1);
  lua_remove(vm, 1);
  lux_push<RET>(vm, (obj->*fun)(lux_to<ARG0>(vm,0 +1)));
  return 1;
 }

template <class ARG0, class ARG1>
 static int lux_thunk(lua_State *vm, void fun(ARG0, ARG1))
 {
  fun(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1));
  return 0;
 }

template <class ARG0, class ARG1, class RET>
 static int lux_thunk(lua_State *vm, RET fun(ARG0, ARG1))
 {
  lux_push<RET>(vm, fun(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1)));
  return 1;
 }

template <class ARG0, class ARG1, class C>
 static int lux_thunk(lua_State *vm, void (C::*fun)(ARG0, ARG1))
 {
  C *obj = lux_to<C*>(vm, 1);
  lua_remove(vm, 1);
  (obj->*fun)(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1));
  return 0;
 }

template <class ARG0, class ARG1, class RET, class C>
 static int lux_thunk(lua_State *vm, RET (C::*fun)(ARG0, ARG1))
 {
  C *obj = lux_to<C*>(vm, 1);
  lua_remove(vm, 1);
  lux_push<RET>(vm, (obj->*fun)(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1)));
  return 1;
 }

template <class ARG0, class ARG1, class ARG2>
 static int lux_thunk(lua_State *vm, void fun(ARG0, ARG1, ARG2))
 {
  fun(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1));
  return 0;
 }

template <class ARG0, class ARG1, class ARG2, class RET>
 static int lux_thunk(lua_State *vm, RET fun(ARG0, ARG1, ARG2))
 {
  lux_push<RET>(vm, fun(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1)));
  return 1;
 }

template <class ARG0, class ARG1, class ARG2, class C>
 static int lux_thunk(lua_State *vm, void (C::*fun)(ARG0, ARG1, ARG2))
 {
  C *obj = lux_to<C*>(vm, 1);
  lua_remove(vm, 1);
  (obj->*fun)(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1));
  return 0;
 }

template <class ARG0, class ARG1, class ARG2, class RET, class C>
 static int lux_thunk(lua_State *vm, RET (C::*fun)(ARG0, ARG1, ARG2))
 {
  C *obj = lux_to<C*>(vm, 1);
  lua_remove(vm, 1);
  lux_push<RET>(vm, (obj->*fun)(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1)));
  return 1;
 }

template <class ARG0, class ARG1, class ARG2, class ARG3>
 static int lux_thunk(lua_State *vm, void fun(ARG0, ARG1, ARG2, ARG3))
 {
  fun(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1));
  return 0;
 }

template <class ARG0, class ARG1, class ARG2, class ARG3, class RET>
 static int lux_thunk(lua_State *vm, RET fun(ARG0, ARG1, ARG2, ARG3))
 {
  lux_push<RET>(vm, fun(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1)));
  return 1;
 }

template <class ARG0, class ARG1, class ARG2, class ARG3, class C>
 static int lux_thunk(lua_State *vm, void (C::*fun)(ARG0, ARG1, ARG2, ARG3))
 {
  C *obj = lux_to<C*>(vm, 1);
  lua_remove(vm, 1);
  (obj->*fun)(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1));
  return 0;
 }
 
template <class ARG0, class ARG1, class ARG2, class ARG3, class RET, class C>
 static int lux_thunk(lua_State *vm, RET (C::*fun)(ARG0, ARG1, ARG2, ARG3))
 {
  C *obj = lux_to<C*>(vm, 1);
  lua_remove(vm, 1);
  lux_push<RET>(vm, (obj->*fun)(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1)));
  return 1;
 }
  
template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4>
 static int lux_thunk(lua_State *vm, void fun(ARG0, ARG1, ARG2, ARG3, ARG4))
 {
  fun(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1), lux_to<ARG4>(vm,4 +1));
  return 0;
 }
   
template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4, class RET>
 static int lux_thunk(lua_State *vm, RET fun(ARG0, ARG1, ARG2, ARG3, ARG4))
 {
  lux_push<RET>(vm, fun(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1), lux_to<ARG4>(vm,4 +1)));
  return 1;
 }

template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4, class C>
 static int lux_thunk(lua_State *vm, void (C::*fun)(ARG0, ARG1, ARG2, ARG3, ARG4))
 {
  C *obj = lux_to<C*>(vm, 1);
  lua_remove(vm, 1);
  (obj->*fun)(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1), lux_to<ARG4>(vm,4 +1));
  return 0;
 }
 
template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4, class RET, class C>
 static int lux_thunk(lua_State *vm, RET (C::*fun)(ARG0, ARG1, ARG2, ARG3, ARG4))
 {
  C *obj = lux_to<C*>(vm, 1);
  lua_remove(vm, 1);
  lux_push<RET>(vm, (obj->*fun)(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1), lux_to<ARG4>(vm,4 +1)));
  return 1;
 }
 
template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5>
 static int lux_thunk(lua_State *vm, void fun(ARG0, ARG1, ARG2, ARG3, ARG4, ARG5))
 {
  fun(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1), lux_to<ARG4>(vm,4 +1), lux_to<ARG5>(vm,5 +1));
  return 0;
 }
 
template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class RET>
 static int lux_thunk(lua_State *vm, RET fun(ARG0, ARG1, ARG2, ARG3, ARG4, ARG5))
 {
  lux_push<RET>(vm, fun(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1), lux_to<ARG4>(vm,4 +1), lux_to<ARG5>(vm,5 +1)));
  return 1;
 }
 
template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class C>
 static int lux_thunk(lua_State *vm, void (C::*fun)(ARG0, ARG1, ARG2, ARG3, ARG4, ARG5))
 {
  C *obj = lux_to<C*>(vm, 1);
  lua_remove(vm, 1);
  (obj->*fun)(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1), lux_to<ARG4>(vm,4 +1), lux_to<ARG5>(vm,5 +1));
  return 0;
 }
 
template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class RET, class C>
 static int lux_thunk(lua_State *vm, RET (C::*fun)(ARG0, ARG1, ARG2, ARG3, ARG4, ARG5))
 {
  C *obj = lux_to<C*>(vm, 1); 
  lua_remove(vm, 1); 
  lux_push<RET>(vm, (obj->*fun)(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1), lux_to<ARG4>(vm,4 +1), lux_to<ARG5>(vm,5 +1))); 
  return 1; 
 } 
 
template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6>
 static int lux_thunk(lua_State *vm, void fun(ARG0, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6)) 
 {
  fun(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1), lux_to<ARG4>(vm,4 +1), lux_to<ARG5>(vm,5 +1), lux_to<ARG6>(vm,6 +1));
  return 0;
 } 
 
template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class RET>
 static int lux_thunk(lua_State *vm, RET fun(ARG0, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6))
 {
  lux_push<RET>(vm, fun(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1), lux_to<ARG4>(vm,4 +1), lux_to<ARG5>(vm,5 +1), lux_to<ARG6>(vm,6 +1))); 
  return 1; 
 }
  
template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class C>
 static int lux_thunk(lua_State *vm, void (C::*fun)(ARG0, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6)) 
 {
  C *obj = lux_to<C*>(vm, 1); 
  lua_remove(vm, 1); 
  (obj->*fun)(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1), lux_to<ARG4>(vm,4 +1), lux_to<ARG5>(vm,5 +1), lux_to<ARG6>(vm,6 +1)); 
  return 0; 
 } 
 
template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class RET, class C>
 static int lux_thunk(lua_State *vm, RET (C::*fun)(ARG0, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6))
 {
  C *obj = lux_to<C*>(vm, 1); 
  lua_remove(vm, 1); 
  lux_push<RET>(vm, (obj->*fun)(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1), lux_to<ARG4>(vm,4 +1), lux_to<ARG5>(vm,5 +1), lux_to<ARG6>(vm,6 +1))); 
  return 1; 
 } 
 
template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class ARG7>
 static int lux_thunk(lua_State *vm, void fun(ARG0, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7)) 
 {
  fun(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1), lux_to<ARG4>(vm,4 +1), lux_to<ARG5>(vm,5 +1), lux_to<ARG6>(vm,6 +1), lux_to<ARG7>(vm,7 +1));
  return 0; 
 }
 
template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class ARG7, class RET>
 static int lux_thunk(lua_State *vm, RET fun(ARG0, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7)) 
 {
  lux_push<RET>(vm, fun(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1), lux_to<ARG4>(vm,4 +1), lux_to<ARG5>(vm,5 +1), lux_to<ARG6>(vm,6 +1), lux_to<ARG7>(vm,7 +1))); 
  return 1; 
 }
 
template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class ARG7, class C>
 static int lux_thunk(lua_State *vm, void (C::*fun)(ARG0, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7)) 
 {
  C *obj = lux_to<C*>(vm, 1); 
  lua_remove(vm, 1); 
  (obj->*fun)(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1), lux_to<ARG4>(vm,4 +1), lux_to<ARG5>(vm,5 +1), lux_to<ARG6>(vm,6 +1), lux_to<ARG7>(vm,7 +1)); 
  return 0; 
 } 
 
template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class ARG7, class RET, class C>
 static int lux_thunk(lua_State *vm, RET (C::*fun)(ARG0, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7)) 
 {
  C *obj = lux_to<C*>(vm, 1); 
  lua_remove(vm, 1); 
  lux_push<RET>(vm, (obj->*fun)(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1), lux_to<ARG4>(vm,4 +1), lux_to<ARG5>(vm,5 +1), lux_to<ARG6>(vm,6 +1), lux_to<ARG7>(vm,7 +1))); 
  return 1; 
 }
 
template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class ARG7, class ARG8>
 static int lux_thunk(lua_State *vm, void fun(ARG0, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8)) 
 {
  fun(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1), lux_to<ARG4>(vm,4 +1), lux_to<ARG5>(vm,5 +1), lux_to<ARG6>(vm,6 +1), lux_to<ARG7>(vm,7 +1), lux_to<ARG8>(vm,8 +1)); 
  return 0; 
 } 
 
template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class ARG7, class ARG8, class RET>
 static int lux_thunk(lua_State *vm, RET fun(ARG0, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8)) 
 {
  lux_push<RET>(vm, fun(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1), lux_to<ARG4>(vm,4 +1), lux_to<ARG5>(vm,5 +1), lux_to<ARG6>(vm,6 +1), lux_to<ARG7>(vm,7 +1), lux_to<ARG8>(vm,8 +1))); 
  return 1; 
 }
 
template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class ARG7, class ARG8, class C>
 static int lux_thunk(lua_State *vm, void (C::*fun)(ARG0, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8)) 
 {
  C *obj = lux_to<C*>(vm, 1); 
  lua_remove(vm, 1); 
  (obj->*fun)(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1), lux_to<ARG4>(vm,4 +1), lux_to<ARG5>(vm,5 +1), lux_to<ARG6>(vm,6 +1), lux_to<ARG7>(vm,7 +1), lux_to<ARG8>(vm,8 +1)); 
  return 0; 
 } 
 
template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class ARG7, class ARG8, class RET, class C>
 static int lux_thunk(lua_State *vm, RET (C::*fun)(ARG0, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8)) 
 {
  C *obj = lux_to<C*>(vm, 1); 
  lua_remove(vm, 1); 
  lux_push<RET>(vm, (obj->*fun)(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1), lux_to<ARG4>(vm,4 +1), lux_to<ARG5>(vm,5 +1), lux_to<ARG6>(vm,6 +1), lux_to<ARG7>(vm,7 +1), lux_to<ARG8>(vm,8 +1))); 
  return 1; 
 } 
 
template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class ARG7, class ARG8, class ARG9>
 static int lux_thunk(lua_State *vm, void fun(ARG0, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9)) 
 {
  fun(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1), lux_to<ARG4>(vm,4 +1), lux_to<ARG5>(vm,5 +1), lux_to<ARG6>(vm,6 +1), lux_to<ARG7>(vm,7 +1), lux_to<ARG8>(vm,8 +1), lux_to<ARG9>(vm,9 +1)); 
  return 0; 
 } 
 
template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class ARG7, class ARG8, class ARG9, class RET>
 static int lux_thunk(lua_State *vm, RET fun(ARG0, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9)) 
 {
  lux_push<RET>(vm, fun(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1), lux_to<ARG4>(vm,4 +1), lux_to<ARG5>(vm,5 +1), lux_to<ARG6>(vm,6 +1), lux_to<ARG7>(vm,7 +1), lux_to<ARG8>(vm,8 +1), lux_to<ARG9>(vm,9 +1))); 
  return 1; 
 } 
 
template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class ARG7, class ARG8, class ARG9, class C>
 static int lux_thunk(lua_State *vm, void (C::*fun)(ARG0, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9)) 
 {
  C *obj = lux_to<C*>(vm, 1);
  lua_remove(vm, 1);
  (obj->*fun)(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1), lux_to<ARG4>(vm,4 +1), lux_to<ARG5>(vm,5 +1), lux_to<ARG6>(vm,6 +1), lux_to<ARG7>(vm,7 +1), lux_to<ARG8>(vm,8 +1), lux_to<ARG9>(vm,9 +1)); 
  return 0; 
 } 
 
template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class ARG7, class ARG8, class ARG9, class RET, class C> 
 static int lux_thunk(lua_State *vm, RET (C::*fun)(ARG0, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9)) 
 {
  C *obj = lux_to<C*>(vm, 1); 
  lua_remove(vm, 1); 
  lux_push<RET>(vm, (obj->*fun)(lux_to<ARG0>(vm,0 +1), lux_to<ARG1>(vm,1 +1), lux_to<ARG2>(vm,2 +1), lux_to<ARG3>(vm,3 +1), lux_to<ARG4>(vm,4 +1), lux_to<ARG5>(vm,5 +1), lux_to<ARG6>(vm,6 +1), lux_to<ARG7>(vm,7 +1), lux_to<ARG8>(vm,8 +1), lux_to<ARG9>(vm,9 +1))); 
  return 1; 
 }

#endif // file

