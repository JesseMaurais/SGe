#include <cstdlib>
#include "lux.hpp" 


int main(int argc, char **argv)
{
	lua_State *vm = luaL_newstate();
	luaL_openlibs(vm);
	lux_throwatpanic(vm);

	printf("This should fail immediately\n");
	try {
	 luaL_error(vm, "Wrench in the works!");
	}
	catch (lux_Exception &e) {
	 printf("%s\n", e.what());
	 lua_pop(vm, 1);
	}
	catch (...) {
	 puts("WTF?\n");
	}

	printf("This should fail again\n");
	try {
	 luaL_dofile(vm, "test5.lua");
	}
	catch (lux_Exception &e) {
	 printf("%s\n", e.what());
	}
	catch (...) {
	 puts("!?\n");
	}

	lux_stackdump(vm);
	lua_close(vm);
}

