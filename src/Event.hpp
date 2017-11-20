#ifndef Event_hpp
#define Event_hpp

#include <functional>

union SDL_Event;

enum UserEventType
{
	EscapeEvent,
	ExecuteCommand,
	UpdateOpenAL,
	UpdateOpenCL,
	UpdateOpenGL,
	UserEventCount,
};

using EventHandler = std::function<void(SDL_Event const &event)>;

class ScopedEventHandler
{
public:

	ScopedEventHandler() = default;
	ScopedEventHandler(unsigned type, EventHandler handler);
	~ScopedEventHandler();

private:

	unsigned event = 0;
};

namespace SDL
{
	unsigned UserEvent(enum UserEventType type);
	bool SendUserEvent(enum UserEventType type, unsigned code = 0, char *string = nullptr);
	void ProcessEvents();
}

#endif // file
