#include "Event.hpp"
#include "SDL.hpp"
#include <stack>
#include <map>

unsigned UserEvent(enum UserEventType type)
{
	static const auto base = SDL_RegisterEvents(UserEventCount);
	return static_cast<unsigned>(base + type);
}

using Stack = std::stack<EventHandler>;
using StackMap = std::map<Uint32, Stack>;

static Stack &HandlerStack(Uint32 eventCode)
{
	static StackMap eventStacks;
	return eventStacks[eventCode];
}

void Dispatch(const SDL_Event &event)
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
}

void PushEventHandler(unsigned eventCode, EventHandler function)
{
	HandlerStack(eventCode).push(function);
}

void PopEventHandler(unsigned eventCode)
{
	HandlerStack(eventCode).pop();
}
