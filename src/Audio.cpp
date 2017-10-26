#include "Audio.hpp"
#include "Source.hpp"
#include "SDL.hpp"

AudioBuffer::AudioBuffer()
{
	id = Manager().Add(this);
}

AudioBuffer::~AudioBuffer()
{
	SDL_verify(Manager().Remove(id) == this);
}

AudioSource::AudioSource()
{
	id = Manager().Add(this);
}

AudioSource::~AudioSource()
{
	SDL_verify(Manager().Remove(id) == this);
}

