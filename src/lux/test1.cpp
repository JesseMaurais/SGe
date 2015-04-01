#include <cstdlib>
#include "lux.hpp" 


void HelloWorld(void)
{
	printf("Hello World!\n");
}

int Add(int a, int b)
{
	return a + b;
}

void Repeat(int a, int b, int c)
{
	printf("%d, %d, %d\n", a, b, c);
}

int main(int argc, char **argv)
{
	lua_State *vm = luaL_newstate();
	luaL_openlibs(vm);
	
	lux_function(vm, HelloWorld);
	lux_function(vm, Repeat);
	lux_function(vm, Add);

	luaL_dofile(vm, "test1.lua");
	lux_stackdump(vm);
	lua_close(vm);
}


