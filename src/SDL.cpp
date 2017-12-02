#include "SDL.hpp"
#include "std.hpp"
#include "Error.hpp"
#include "Strings.hpp"
#include <cstring>
#include <cerrno>


bool SDL::Init(Uint32 const flags)
{
	if (0 == SDL_Init(flags))
	{
		if (0 == std::atexit(SDL_Quit))
		{
			return true;
		}
		SDL::SetError(CannotMakeExit, "SDL_Quit");
	}
	return false;
}

bool SDL::LogError(char const *origin, const char *error)
{
	SDL::Log(ColonSeparator, origin, error);
	return true;
}

bool SDL::LogError(char const *origin)
{
	auto error = SDL_GetError();
	if (error)
	{
		return SDL::LogError(origin, error);
	}
	return false;
}

bool SDL::SetErrno(int const error)
{
	if (error)
	{
		return SDL::SetError(std::strerror(error));
	}
	return false;
}

bool SDL::SetErrno()
{
	return SDL::SetErrno(errno);
}

namespace
{
	constexpr unsigned ButtonCount = 5;

	SDL_MessageBoxButtonData const buttons[ButtonCount] =
	{
	{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, SDL_ASSERTION_IGNORE, String(MessageBoxContinue) },
	{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, SDL_ASSERTION_ABORT, String(MessageBoxAbort) },
	{ 0, SDL_ASSERTION_RETRY, String(MessageBoxRetry) },
	{ 0, SDL_ASSERTION_BREAK, String(MessageBoxBreak) },
	{ 0, SDL_ASSERTION_ALWAYS_IGNORE, String(MessageBoxIgnore) },
	};

	SDL_MessageBoxColorScheme const colorScheme =
	{
		{
			{  47,  52,  63 }, // SDL_MESSAGEBOX_COLOR_BACKGROUND
			{ 211, 218, 227 }, // SDL_MESSAGEBOX_COLOR_TEXT
			{  56,  60,  74 }, // SDL_MESSAGEBOX_COLOR_BUTTON_BORDER
			{  64,  69,  82 }, // SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND
			{  64, 132, 214 }, // SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED
		}
	};

	class MessageBox : SDL_MessageBoxData
	{
	public:

		MessageBox(std::string const &text, SDL_Window *window, SDL_MessageBoxFlags const flags)
		{
			this->flags = flags;
			this->numbuttons = ButtonsUsedFor(flags);
			this->buttons = buttons;
			this->colorScheme = colorScheme;
			this->message = text.c_str();
			this->title = TitleUsedFor(flags);
			this->window = window;
		}

		operator SDL_AssertState() const
		{
			int id = SDL_ASSERTION_ABORT;
			if (SDL_ShowMessageBox(this, &id))
			{
				auto const error = SDL_GetError();
				if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, error, window))
				{
					SDL::LogError(CannotShowMessageBox);
				}
				SDL::LogError(title, message);
			}
			return static_cast<SDL_AssertState>(id);
		}

	private:

		static int ButtonsUsedFor(SDL_MessageBoxFlags flags)
		{
			switch (flags)
			{
			case SDL_MESSAGEBOX_ERROR:
				return 5; // Continue, Abort, Retry, Break, Ignore
			case SDL_MESSAGEBOX_WARNING:
				return 4; // Continue, Abort, Retry, Break
			case SDL_MESSAGEBOX_INFORMATION:
				return 1; // Continue
			default:
				return 0;
			}
		}

		static char const *TitleUsedFor(SDL_MessageBoxFlags flags)
		{
			switch (flags)
			{
			case SDL_MESSAGEBOX_ERROR:
				return String(MessageBoxError);
			case SDL_MESSAGEBOX_WARNING:
				return String(MessageBoxWarning);
			case SDL_MESSAGEBOX_INFORMATION:
				return String(MessageBoxInformation);
			default:
				return String(None);
			}
		}
	};

	SDL_AssertState AssertionHandler(SDL_AssertData const *data, void *user)
	{
		if (data->always_ignore)
		{
			return SDL_ASSERTION_IGNORE;
		}

		std::string message;
		stl::sprintf
		( message
		, "Assert failed at %1 in %2 on line %3, %4 times\n\t%5\n"
		, data->function
		, data->filename
		, data->linenum
		, data->trigger_count
		, data->condition
		);

		union {
		 SDL_Window *window;
		 void *address;
		};
		if (user) address = user;
		return MessageBox(message, window, SDL_MESSAGEBOX_ERROR);
	}
}

bool SDL::ShowError(SDL_MessageBoxFlags const flags, SDL_Window *window)
{
	auto message = SDL_GetError();
	if (message)
	{
		SDL_AssertState const state = MessageBox(message, window, flags);
		switch (state)
		{
		case SDL_ASSERTION_ABORT:
			std::abort();
			// no break
		case SDL_ASSERTION_BREAK:
			SDL_TriggerBreakpoint();
			// no break
		case SDL_ASSERTION_IGNORE:
		case SDL_ASSERTION_ALWAYS_IGNORE:
			break;
		case SDL_ASSERTION_RETRY:
			return true;
		}
	}
	return false;
}

void SDL::SetAssertHandler(SDL_Window *window)
{
	SDL_SetAssertionHandler(AssertionHandler, window);
}

void SDL::ResetAssertHandler()
{
	SDL_ResetAssertionReport();
	SDL_AssertionHandler handler = SDL_GetDefaultAssertionHandler();
	SDL_SetAssertionHandler(handler, nullptr);
}

SDL::ScopedAssertHandler::ScopedAssertHandler(SDL_Window *window)
{
	handler = SDL_GetAssertionHandler(&userdata);
	SDL::SetAssertHandler(window);
}

SDL::ScopedAssertHandler::~ScopedAssertHandler()
{
	SDL_SetAssertionHandler(handler, userdata);
}
