#include "Audio.hpp"
#include "Source.hpp"
#include "SDL.hpp"

Resource &AudioBufferResource();

AudioBuffer::AudioBuffer()
{
	id = AudioBufferResource().Add(this);
}

AudioBuffer::~AudioBuffer()
{
	SDL_verify(AudioBufferResource().Remove(id) == this);
}

Resource &AudioSourceResource();

AudioSource::AudioSource()
{
	id = AudioSourceResource().Add(this);
}

AudioSource::~AudioSource()
{
	SDL_verify(AudioSourceResource().Remove(id) == this);
}

