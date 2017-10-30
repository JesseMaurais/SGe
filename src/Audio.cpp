#include "Audio.hpp"
#include "Source.hpp"
#include "Assert.hpp"
#include "SDL.hpp"

AudioBuffer::AudioBuffer()
{
	id = Manager().Add(this);
}

AudioBuffer::~AudioBuffer()
{
	verify(Manager().Remove(id) == this);
}

AudioSource::AudioSource()
{
	id = Manager().Add(this);
}

AudioSource::~AudioSource()
{
	verify(Manager().Remove(id) == this);
}

