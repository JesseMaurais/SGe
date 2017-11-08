#ifndef Kernel_hpp
#define Kernel_hpp

#include "Source.hpp"
#include <memory>

class Kernel : public ManagedSource<Kernel>
{
public:

	static Resources &Manager();

protected:

	bool UpdateSource() override;

private:

	class Program;
	std::shared_ptr<Program> source;
	std::string options;
};

#endif // file
