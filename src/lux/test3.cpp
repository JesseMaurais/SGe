#include <sys/stat.h>
#include "lux.hpp" 


struct MyRecord
{
	char byte;
	short word;
	double real;
	union {
	 long u32;
	 short u16;
	};
};

template <> const char *lux_Record<MyRecord>::Type::name = "Record";

int main(int argc, char **argv)
{
	lua_State *vm = luaL_newstate();
	luaL_openlibs(vm);

	lux_Record<MyRecord>::open(vm);
	lux_member(MyRecord, byte);
	lux_member(MyRecord, word);
	lux_member(MyRecord, real);
	lux_member(MyRecord, u32);
	lux_member(MyRecord, u16);

	luaL_dofile(vm, "test3.lua");
	lux_stackdump(vm);
	lua_close(vm);
}


