#ifndef Audio_hpp
#define Audio_hpp

#include "Source.hpp"

class AudioBuffer : public Source
{
protected:

	unsigned id;

private:

	void Rename(unsigned id) override
	{
		this->id = id;
	}

public:

	AudioBuffer();
	~AudioBuffer();
};

class AudioSource : public Source
{
protected:

	unsigned id;

private:

	void Rename(unsigned id) override
	{
		this->id = id;
	}

public:

	AudioSource();
	~AudioSource();
};

#endif // file
