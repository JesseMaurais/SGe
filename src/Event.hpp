#ifndef __Event__
#define __Event__

#include "GUI.hpp"

extern const Uint32 GUI_EVENT;

void Process(SDL_UserEvent &event);

struct UserEvent
{
	virtual void Event(GUI *obj, int code)=0;
	signed Send(GUI *obj, int code);
};

signed Send(GUI *obj, UserEvent *user, int code);

template <class C> signed Send(C *obj, void (C::*event)(int code), int code)
{
	struct Userdata : UserEvent
	{
		void (C::*event)(int code);

		void Event(GUI *obj, int code)
		{
			C *user = dynamic_cast<C*>(obj);
			(user->*event)(code);
			delete this;
		}

		Userdata(void (C::*event)(int code))
		{
			this->event = event;
		}
	};

	return Send(obj, new Userdata(event), code);
}

#endif // file

