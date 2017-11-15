#include "Event.hpp"
#include "SDL.hpp"
#include <cassert>
#include <stack>
#include <map>


unsigned SDL::UserEvent(enum UserEventType type)
{
	assert(type < UserEventCount);
	static Uint32 const base = SDL_RegisterEvents(UserEventCount);
	return static_cast<unsigned>(type) + base;
}

bool SDL::SendUserEvent(enum UserEventType type, unsigned code)
{
	SDL_Event event;
	event.user.type = SDL::UserEvent(type);
	event.user.code = code;
	return 0 == SDL_PushEvent(&event);
}

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

	bool Dispatch(const SDL_Event &event)
	{
		Stack store;
		Stack &stack = HandlerStack(event.type);
		while (not stack.empty())
		{
			EventHandler handler = stack.top();
			if (not handler(event))
			{
				store.push(handler);
				stack.pop();
			}
			else break;
		}
		while (not store.empty())
		{
			EventHandler handler = store.top();
			stack.push(handler);
			store.pop();
		}
		return false;
	}
}

void SDL::ProcessEvents()
{
	static SDL_Event event;
	bool done = false;
	do
	{
		if (SDL_WaitEvent(&event))
		{
			done = Dispatch(event);
		}
		else
		{
			SDL::perror("SDL_WaitEvent");
		}

		done |= SDL_FALSE == SDL_QuitRequested();
	}
	while (not done);
}

ScopedEventHandler::ScopedEventHandler(UserEventType type, EventHandler handler)
	: ScopedEventHandler(SDL::UserEvent(type), handler)
{}

ScopedEventHandler::ScopedEventHandler(unsigned type, EventHandler handler) : event(type)
{
	PushEventHandler(event, handler);
}

ScopedEventHandler::~ScopedEventHandler()
{
	PopEventHandler(event);
}
