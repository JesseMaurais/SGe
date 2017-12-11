#include "Event.hpp"
#include "Error.hpp"
#include "SDL.hpp"
#include <exception>
#include <cassert>
#include <stack>
#include <map>

namespace
{
	using Stack = std::stack<EventHandler>;
	using StackMap = std::map<Uint32, Stack>;

	Stack &HandlerStack(Uint32 eventCode)
	{
		static StackMap singleton;
		return singleton[eventCode];
	}

	void PushEventHandler(unsigned eventCode, EventHandler function)
	{
		HandlerStack(eventCode).push(function);
	}

	void PopEventHandler(unsigned eventCode)
	{
		HandlerStack(eventCode).pop();
	}

	void Dispatch(SDL_Event const &event)
	{
		Stack const &stack = HandlerStack(event.type);
		if (not stack.empty())
		{
			stack.top()(event);
		}
	}
}

void SDL::ProcessEvents()
{
	bool done = false;
	// Allow loop to break
	ScopedEventHandler escape
	(
		SDL::UserEvent(EscapeEvent), [&](SDL_Event const &event)
		{
			assert(SDL::UserEvent(EscapeEvent) == event.type);
			done = true;
		}
	);
	// Main event loop
	SDL_Event event;
	while (not done) try
	{
		// Block until event in queue
		if (not SDL_WaitEvent(&event))
		{
			SDL::LogError("SDL_WaitEvent");
		}
		else
		{
			Dispatch(event);
		}
		// Break when we receive a quit event
		done |= SDL_TRUE == SDL_QuitRequested();
	}
	catch (std::exception const &exception)
	{
		SDL::SetError(CaughtException, exception.what());
		// Break if the user chooses to after an exception
		done |= not SDL::ShowError(SDL_MESSAGEBOX_WARNING);
	}
}


unsigned SDL::UserEvent(enum UserEventType type)
{
	assert(type < UserEventCount);
	static Uint32 const base = SDL_RegisterEvents(UserEventCount);
	return static_cast<unsigned>(type) + base;
}

bool SDL::SendUserEvent(enum UserEventType type, unsigned code, char *data, std::size_t size)
{
	SDL_Event event;
	event.user.type = SDL::UserEvent(type);
	event.user.code = code;
	event.user.data1 = data;
	event.user.data2 = data + size;
	return 0 == SDL_PushEvent(&event);
}

char *SDL::GetUserEventData(SDL_Event const &event, std::size_t &size)
{
	char *begin = static_cast<char*>(event.user.data1);
	char *end = static_cast<char*>(event.user.data2);
	assert(begin <= end);
	size = end - begin;
	return begin;
}

bool SDL::SendUserEvent(enum UserEventType type, unsigned code, std::string const &string)
{
	// Use portable strdup since its not C
	char *data = SDL_strdup(string.c_str());
	if (not data)
	{
		// Not all systems will set the error number
		SDL::SetErrno() or SDL::SetError(OutOfMemory);
		return false; // not sent
	}
	return SDL::SendUserEvent(type, code, data, string.size());
}

std::string SDL::GetUserEventData(SDL_Event const &event)
{
	std::size_t size;
	char *data = SDL::GetUserEventData(event, size);
	if (data)
	{
		std::string string(data, size);
		std::free(data);
		return string;
	}
	return std::string();
}

ScopedEventHandler::ScopedEventHandler(unsigned type, EventHandler handler)
: event(type)
{
	PushEventHandler(event, handler);
}

ScopedEventHandler::~ScopedEventHandler()
{
	PopEventHandler(event);
}
