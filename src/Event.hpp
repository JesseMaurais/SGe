#ifndef Event_hpp
#define Event_hpp

#include <functional>

union SDL_Event;

enum UserEventType
{
	EscapeEvent,
	EvaluateScript,
	UpdateFile,
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
	void ProcessEvents();
	unsigned UserEvent(enum UserEventType type);
	bool SendUserEvent(enum UserEventType type, unsigned code = 0, char const *data = nullptr, std::size_t size = 0);
	char *GetUserEventData(SDL_Event const &event, std::size_t &size);

	inline bool SendUserEvent(enum UserEventType type, unsigned code, std::string_view string)
	{
		return SendUserEvent(type, code, string.data(), string.size());
	}
	std::string GetUserEventData(SDL_Event const &event);
}

#endif // file
