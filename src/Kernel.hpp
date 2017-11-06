#ifndef Kernel_hpp
#define Kernel_hpp

#include "Source.hpp"
#include <memory>

class Kernel : public ManagedSource<Kernel>
{
public:

	class Program : public ManagedSource<Program>
	{
	public:

		static Resources &Manager();
	};

	static Resources &Manager();

protected:

	void UpdateSource() override;

private:

	std::shared_ptr<Program> program;
	std::string options;
};

#endif // file
