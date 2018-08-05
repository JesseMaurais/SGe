// Test for an internal compiler error in GCC 8.2.0
#include "fdbuf.cpp"
#include "fd.cpp"
#include "ipc.cpp"
#include "auto.cpp"

int main(char **argv, int argc)
{
	sys::io::pstream ls("ls");
	std::string line;
	while (std::getline(ls, line))
	{
		std::cout << line << std::endl;
	}
	return 0;
}

