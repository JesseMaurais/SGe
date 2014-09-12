#include "Event.hpp"

const Uint32 GUI_EVENT = SDL_RegisterEvents(1);

signed UserEvent::Send(GUI *obj, int code)
{
	return ::Send(obj, this, code);
}

signed Send(GUI *obj, UserEvent *user, int code)
{
	SDL_Event event;
	event.type = GUI_EVENT;
	event.user.code = code;
	event.user.data1 = reinterpret_cast<void*>(obj);
	event.user.data2 = reinterpret_cast<void*>(user);
	return SDL_PushEvent(&event);
}

void Process(SDL_UserEvent &event)
{
	GUI *obj = reinterpret_cast<GUI*>(event.data1);
	UserEvent *user = reinterpret_cast<UserEvent*>(event.data2);
	user->Event(obj, event.code);
}

