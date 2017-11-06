#ifndef Event_hpp
#define Event_hpp

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

namespace SDL
{
	unsigned UserEvent(enum UserEventType type);
	bool SendUserEvent(enum UserEventType type, unsigned code = 0);

	using EventHandler = bool(*)(SDL_Event const &event);
	void PushEventHandler(unsigned type, EventHandler function);
	void PopEventHandler(unsigned type);
	void Dispatch(const SDL_Event &event);

	inline void PushEventHandler(UserEventType type, EventHandler function)
	{
		PushEventHandler(UserEvent(type), function);
	}

	inline void PopEventHandler(UserEventType type)
	{
		PopEventHandler(UserEvent(type));
	}
}

#endif // file
