#ifndef Audio_hpp
#define Audio_hpp

#include "Source.hpp"

class AudioBuffer : public Source
{
protected:

	unsigned id;

public:

	AudioBuffer();
	~AudioBuffer();
};

class AudioSource : public Source
{
protected:

	unsigned id;

public:

	AudioSource();
	~AudioSource();
};

#endif // file
