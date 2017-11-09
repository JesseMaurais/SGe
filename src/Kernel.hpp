#ifndef Kernel_hpp
#define Kernel_hpp

#include <memory>

class Kernel
{
public:

private:

	class Program;
	std::unique_ptr<Program> source;
	std::string options;
};

#endif // file
