#ifndef Event_hpp
#define Event_hpp

enum UserEventType { UpdateSpace, ExecuteCommand, UserEventCount };
unsigned UserEvent(enum UserEventType type);

union SDL_Event;
using EventHandler = bool(*)(const SDL_Event &event);
void PushEventHandler(unsigned type, EventHandler function);
void PopEventHandler(unsigned type);
void Dispatch(const SDL_Event &event);

#endif // file
