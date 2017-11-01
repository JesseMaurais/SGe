#ifndef Event_hpp
#define Event_hpp

enum UserEventType
{
	UpdateSpace,
	UpdateAudio,
	UpdateVideo,
	ExecuteCommand,
	UserEventCount
};

bool SendUserEvent(enum UserEventType type, unsigned code);
unsigned UserEvent(enum UserEventType type);

union SDL_Event;
using EventHandler = bool(*)(SDL_Event const &event);
void PushEventHandler(unsigned type, EventHandler function);
void PopEventHandler(unsigned type);
void Dispatch(const SDL_Event &event);

#endif // file
