#ifndef Audio_hpp
#define Audio_hpp

#include "Source.hpp"

class AudioBuffer : public SourceCommon<AudioBuffer>
{
public:

	static Resources &Manager();
};

class AudioSource : public SourceCommon<AudioSource>
{
public:

	static Resources &Manager();
};

#endif // file
