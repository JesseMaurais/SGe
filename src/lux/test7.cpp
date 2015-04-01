#include <cstdlib>
#include "lux.hpp" 


int Reverse(lua_State *vm)
{
	int a, b, c;
	lux_to(vm, 1, &a, &b, &c);
	printf("C++ %d, %d, %d\n", a, b, c);
	return lux_push(vm, c, b, a);
}

int main(int argc, char **argv)
{
	lua_State *vm = luaL_newstate();
	luaL_openlibs(vm);

	lua_register(vm, "Reverse", Reverse);

	luaL_dofile(vm, "test7.lua");

	printf("Now let's try to call Lua code from C++\n");
	lux_call(vm, "Callback", 3, 2, 1);

	lux_stackdump(vm);
	lua_close(vm);
}


