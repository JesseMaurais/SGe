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
		static StackMap eventStacks;
		return eventStacks[eventCode];
	}

	void PushEventHandler(unsigned eventCode, EventHandler function)
	{
		HandlerStack(eventCode).push(function);
	}

	void PopEventHandler(unsigned eventCode)
	{
		HandlerStack(eventCode).pop();
	}

	void DispatchEvent(const SDL_Event &event)
	{
		Stack const &stack = HandlerStack(event.type);
		if (not stack.empty())
		{
			stack.top()(event);
		}
	}

	bool SendUserEvent(enum UserEventType type, unsigned code, char *data, std::size_t size)
	{
		SDL_Event event;
		event.user.type = SDL::UserEvent(type);
		event.user.code = code;
		event.user.data1 = data;
		event.user.data2 = data + size;
		return 0 == SDL_PushEvent(&event);
	}

	char *UserEventData(SDL_Event const &event, std::size_t &size)
	{
		char *begin = static_cast<char*>(event.user.data1);
		char *end = static_cast<char*>(event.user.data2);
		assert(begin <= end);
		size = end - begin;
		return begin;
	}
}

void SDL::ProcessEvents()
{
	bool done = false;
	ScopedEventHandler escape(
		SDL::UserEvent(EscapeEvent), [&done](SDL_Event const &event)
		{
			(void) event;
			done = true;
		}
	);
	SDL_Event event;
	do try
	{
		if (SDL_WaitEvent(&event))
		{
			DispatchEvent(event);
		}
		else
		{
			SDL::LogError("SDL_WaitEvent");
		}

		done |= SDL_QuitRequested() == SDL_TRUE;
	}
	catch (std::exception const &exception)
	{
		SDL::SetError(CaughtException, exception.what());
		done |= not SDL::ShowError(SDL_MESSAGEBOX_WARNING);
	}
	while (not done);
}


unsigned SDL::UserEvent(enum UserEventType type)
{
	assert(type < UserEventCount);
	static Uint32 const base = SDL_RegisterEvents(UserEventCount);
	return static_cast<unsigned>(type) + base;
}

bool SDL::SendUserEvent(enum UserEventType type, unsigned code)
{
	return ::SendUserEvent(type, code, nullptr, 0);
}

bool SDL::SendUserEvent(enum UserEventType type, unsigned code, std::string const &string)
{
	char *data = SDL_strdup(string.c_str());
	if (not data)
	{
		SDL::SetError(OutOfMemory);
		return false;
	}
	return ::SendUserEvent(type, code, data, string.size());
}

std::string SDL::UserEventData(SDL_Event const &event)
{
	std::size_t size;
	char *data = ::UserEventData(event, size);
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
