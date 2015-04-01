#include <cstdlib>
#include "lux.hpp" 


int main(int argc, char **argv)
{
	lua_State *vm = luaL_newstate();
	luaL_openlibs(vm);
	luxopen_array(vm);

	luaL_dofile(vm, "test2.lua");
	lux_stackdump(vm);
	lua_close(vm);
}


