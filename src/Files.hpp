#ifndef Files_hpp
#define Files_hpp

#include "Source.hpp"

class ManagedFile : public ManagedSource<ManagedFile>
{
public:

	ManagedFile(std::string const &path);

private:

	friend ManagedSource<ManagedFile>;
	static Resources &Manager();
};

#endif // file
