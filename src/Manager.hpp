#ifndef Manager_hpp
#define Manager_hpp

#include "Source.hpp"
#include <vector>

class Manager : public Resource
{
private:

	std::vector<Source*> sources;

public:

	unsigned Add(Source *that) override;
	Source *Remove(unsigned id) override;
	unsigned Update() override;
	unsigned Size();
};

#endif // file
