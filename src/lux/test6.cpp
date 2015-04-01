#include <cstdlib>
#include "lux.hpp"


// Pretend this is the class you already have.

struct Account
{
	Account(void) : balance(0)
	{
		puts("Opening account");
	}

	~Account(void)
	{
		puts("Closing account");
	}

	void Withdrawl(double amount)
	{
		printf("-$%.2f\n", amount);
		balance -= amount;
	}

	void Deposit(double amount)
	{
		printf("+$%.2f\n", amount);
		balance += amount;
	}

	void Print(void)
	{
		printf("$%.2f\n", balance);
	}

 private:

	double balance;
};


// This is the interface you write.

template <> const char *lux_Class<Account>::Type::name = "Account";

template <> void lux_Class<Account>::methods(lua_State *vm, int tab, int meta)
{
	lux_method(vm, tab, Account, Withdrawl);
	lux_method(vm, tab, Account, Deposit);
	lux_method(vm, tab, Account, Print);
}


// You must add it like you would a library.

int main(int argc, char **argv)
{
	lua_State *vm = luaL_newstate();
	luaL_openlibs(vm);

	lux_Class<Account>::open(vm);

	luaL_dofile(vm, "test6.lua");
	lux_stackdump(vm);
	lua_close(vm);
}

