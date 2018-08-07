#include "fdbuf.cpp"
#include "fd.cpp"
#include "ipc.cpp"
#include "fdstream.cpp"

int main(char **argv, int argc)
{
	sys::io::pstream ls("ls");
	std::string line;
	unsigned count = 0;
	while (std::getline(ls, line))
	{
		std::cout << ++count << ": " << line << std::endl;
	}
	return 0;
}

