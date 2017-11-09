#ifndef Video_hpp
#define Video_hpp

#include "Source.hpp"

class VideoBuffer : public ManagedSource<VideoBuffer>
{
public:

	static Resources &Manager();
};

class VideoTexture : public ManagedSource<VideoTexture>
{
public:

	static Resources &Manager();
};

#endif // file
