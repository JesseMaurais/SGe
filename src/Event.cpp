#include "Event.hpp"
#include "Error.hpp"
#include "SDL.hpp"
#include <exception>
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

	void DispatchEvent(const SDL_Event &event)
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
	ScopedEventHandler escape(
		SDL::UserEvent(EscapeEvent), [&done](SDL_Event const &event)
		{
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
			SDL::perror("SDL_WaitEvent");
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

ScopedEventHandler::ScopedEventHandler(unsigned type, EventHandler handler)
: event(type)
{
	PushEventHandler(event, handler);
}

ScopedEventHandler::~ScopedEventHandler()
{
	PopEventHandler(event);
}
