#ifndef Audio_hpp
#define Audio_hpp

#include "Source.hpp"

class AudioBuffer : public Source
{
protected:

	static Resource &Manager();

public:

	AudioBuffer();
	~AudioBuffer();
};

class AudioSource : public Source
{
protected:

	static Resource &Manager();

public:

	AudioSource();
	~AudioSource();
};

#endif // file
