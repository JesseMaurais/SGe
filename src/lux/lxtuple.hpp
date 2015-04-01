#ifndef __lxtuple__
#define __lxtuple__

#include "lxstack.hpp"

template <typename A, typename ... VA>
 int lux_push(lua_State *vm, A a, VA ... va)
{
	lux_push(vm, a);
	lux_push(vm, va...);
	return 1+sizeof...(va);
}

static void lux_to(lua_State *vm, int index)
{
	// stub
}

template <typename A, typename ... VA>
 void lux_to(lua_State *vm, int index, A *a, VA ... va)
{
	*a = lux_to<A>(vm, index);
	lux_to(vm, ++index, va...);
}

template <typename ... Args>
 void lux_call(lua_State *vm, const char *fun, Args ... args)
{
	lua_getglobal(vm, fun);
	lua_call(vm, lux_push(vm, args...), 0);
}


#endif // file

