#ifndef Audio_hpp
#define Audio_hpp

#include "Source.hpp"

class AudioBuffer : public ManagedSource<AudioBuffer>
{
public:

	static Resources &Manager();
};

class AudioSource : public ManagedSource<AudioSource>
{
public:

	static Resources &Manager();
};

#endif // file
