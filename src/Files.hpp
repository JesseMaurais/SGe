#ifndef Files_hpp
#define Files_hpp

#include "Source.hpp"

class FileWatch : ManagedSource<FileWatch>
{
private:

	friend ManagedSource<FileWatch>;
	static Resources &Manager();
};

#endif // file
