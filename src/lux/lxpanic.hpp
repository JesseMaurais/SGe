#ifndef __lxpanic__
#define __lxpanic__

#include <exception>
#include "lua.hpp"

struct lux_Exception : std::exception
{
	static int atpanic(lua_State *vm)
	{
		throw lux_Exception(vm);
		return 0;
	}

	virtual const char *what(void) const throw()
	{
		return lua_tostring(vm, 1);
	}

	explicit lux_Exception(lua_State *state) : vm(state)
		{ /*stub*/ }

	virtual ~lux_Exception(void) throw()
		{ /*stub*/ }

 protected:

	lua_State *vm;
};

#define lux_throwatpanic(vm) lua_atpanic(vm, lux_Exception::atpanic)

#endif // file

