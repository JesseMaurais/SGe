#ifndef Event_hpp
#define Event_hpp

#include <functional>

union SDL_Event;

enum UserEventType
{
	UpdateSpace,
	UpdateOpenAL,
	UpdateOpenCL,
	UpdateOpenGL,
	ExecuteCommand,
	UserEventCount,
};

using EventHandler = std::function<bool(SDL_Event const &event)>;

class ScopedEventHandler
{
public:

	ScopedEventHandler() = default;
	ScopedEventHandler(UserEventType type, EventHandler handler);
	ScopedEventHandler(unsigned type, EventHandler handler);
	~ScopedEventHandler();

private:

	unsigned event = 0;
};

namespace SDL
{
	unsigned UserEvent(enum UserEventType type);
	bool SendUserEvent(enum UserEventType type, unsigned code = 0);
	void ProcessEvents();
}

#endif // file
