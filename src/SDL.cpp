#include "SDL.hpp"
#include "Error.hpp"
#include "Strings.hpp"
#include <sstream>
#include <string>

bool SDL::perror(const char *origin, const char *error)
{
	SDL::Log(ColonSeparator, origin, error);
	return true;
}

bool SDL::perror(const char *origin)
{
	auto error = SDL_GetError();
	if (error)
	{
		return SDL::perror(origin, error);
	}
	return false;
}

namespace
{
	class MessageBox : SDL_MessageBoxData
	{
	public:

		MessageBox(const char *message, SDL_Window *window, SDL_MessageBoxFlags flags)
		{
			constexpr unsigned buttonCount = 5;

			const SDL_MessageBoxButtonData buttons[buttonCount] =
			{
				{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, SDL_ASSERTION_IGNORE, String(MessageBoxContinue) },
				{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, SDL_ASSERTION_ABORT, String(MessageBoxAbort) },
				{ 0, SDL_ASSERTION_RETRY, String(MessageBoxRetry) },
				{ 0, SDL_ASSERTION_BREAK, String(MessageBoxBreak) },
				{ 0, SDL_ASSERTION_ALWAYS_IGNORE, String(MessageBoxIgnore) },
			};

			const SDL_MessageBoxColorScheme colorScheme =
			{
				{
					{  47,  52,  63 }, // SDL_MESSAGEBOX_COLOR_BACKGROUND
					{ 211, 218, 227 }, // SDL_MESSAGEBOX_COLOR_TEXT
					{  56,  60,  74 }, // SDL_MESSAGEBOX_COLOR_BUTTON_BORDER
					{  64,  69,  82 }, // SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND
					{  64, 132, 214 }, // SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED
				}
			};

			this->flags = flags;
			this->numbuttons = ButtonsUsedFor(flags);
			this->buttons = buttons;
			this->colorScheme = &colorScheme;
			this->message = message;
			this->title = TitleUsedFor(flags);
			this->window = window;
		}

		operator SDL_AssertState() const
		{
			int id = SDL_ASSERTION_IGNORE;
			if (SDL_ShowMessageBox(this, &id))
			{
				auto const error = SDL_GetError();
				if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, String(CannotShowMessageBox), error, window))
				{
					SDL::perror(CannotShowMessageBox);
					SDL::perror(title, error);
				}
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
				return 3; // Continue, Abort, Retry
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
}

bool SDL::ShowError(SDL_MessageBoxFlags flags, SDL_Window *window)
{
	auto message = SDL_GetError();
	if (message)
	{
		SDL_AssertState const state = MessageBox(message, window, flags);
		switch (state)
		{
		case SDL_ASSERTION_BREAK:
			SDL_TriggerBreakpoint();
			break;
		case SDL_ASSERTION_IGNORE:
		case SDL_ASSERTION_ALWAYS_IGNORE:
			break;
		case SDL_ASSERTION_ABORT:
			std::abort();
			break;
		case SDL_ASSERTION_RETRY:
			return true;
		}
	}
	return false;
}

void SDL::SetAssertionHandler(SDL_Window *window)
{
	auto handler = [](const SDL_AssertData *data, void *user) -> SDL_AssertState
	{
		if (data->always_ignore) return SDL_ASSERTION_IGNORE;

		std::stringstream stream;
		stream << "Assertion failure at " << data->function;
		stream << "in " << data->filename << " on line " << data->linenum << "), ";
		stream << "triggered " << data->trigger_count << "times\n";
		stream << "       '" << data->condition << "'\n";

		union {
		 SDL_Window *window;
		 void *address;
		};
		if (user) address = user;
		else window = SDL_GetGrabbedWindow();
		auto const string = stream.str();
		auto const message = string.c_str();
		return MessageBox(message, window, SDL_MESSAGEBOX_ERROR);
	};

	SDL_SetAssertionHandler(handler, window);
}

void SDL::ResetAssertionHandler()
{
	SDL_ResetAssertionReport();
	SDL_AssertionHandler handler = SDL_GetDefaultAssertionHandler();
	SDL_SetAssertionHandler(handler, nullptr);
}

