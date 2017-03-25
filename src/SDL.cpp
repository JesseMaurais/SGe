#include "SDL.hpp"
#include "Strings.hpp"
#include <sstream>
#include <string>

class AssertMessageBox : SDL_MessageBoxData
{
	static constexpr unsigned ButtonCount = 5;
	static const SDL_MessageBoxButtonData Buttons[ButtonCount];
	static const SDL_MessageBoxColorScheme ColorScheme;

  public:

	AssertMessageBox(const char *message, const char *title, SDL_Window *window = nullptr)
	{
		this->flags = SDL_MESSAGEBOX_ERROR;
		this->numbuttons = ButtonCount;
		this->buttons = Buttons;
		this->colorScheme = &ColorScheme;
		this->message = message;
		this->title = title;
		this->window = window;
	}

	SDL_AssertState Choose()
	{
		int id = SDL_ASSERTION_ABORT;
		if (SDL_ShowMessageBox(this, &id))
		{
			auto error = SDL_GetError();
			SDL_Log("%s: %s", CannotShowMessageBox, error);

			if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, CannotShowMessageBox, error, window))
			{
				error = SDL_GetError();
				SDL_Log("%s: %s", CannotShowMessageBox, error);
				SDL_Log("%s: %s", title, message);
			}
		}
		return static_cast<SDL_AssertState>(id);
	}
};

const SDL_MessageBoxButtonData AssertMessageBox::Buttons[AssertMessageBox::ButtonCount] =
{
		{ 0, SDL_ASSERTION_RETRY, ErrorMessageBox::Retry },
		{ 0, SDL_ASSERTION_BREAK, ErrorMessageBox::Break },
		{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, SDL_ASSERTION_ABORT, ErrorMessageBox::Abort },
		{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, SDL_ASSERTION_IGNORE, ErrorMessageBox::Ignore },
		{ 0, SDL_ASSERTION_ALWAYS_IGNORE, ErrorMessageBox::AlwaysIgnore }
};

const SDL_MessageBoxColorScheme AssertMessageBox::ColorScheme =
{
	{
		{  47,  52,  63 }, // SDL_MESSAGEBOX_COLOR_BACKGROUND
		{ 211, 218, 227 }, // SDL_MESSAGEBOX_COLOR_TEXT
		{  56,  60,  74 }, // SDL_MESSAGEBOX_COLOR_BUTTON_BORDER
		{  64,  69,  82 }, // SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND
		{  64, 132, 214 }  // SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED
	}
};


static SDL_AssertState AssertionHandler(const SDL_AssertData *data, void *user)
{
	if (data->always_ignore)
	{
		return SDL_ASSERTION_IGNORE;
	}

	const auto quantity = data->trigger_count > 1 ? " times:" : " time:";
	std::stringstream stream;
	stream << "Assertion failure at " << data->function;
	stream << " (" << data->filename << ":" << data->linenum << "), ";
	stream << "triggered " << data->trigger_count << quantity << "\n";
	stream << "       '" << data->condition << "'\n";
	std::string string = stream.str();
	const char *title = data->function;
	const char *message = string.c_str();

	union {
		SDL_Window *window;
		void *address;
	};
	if (user) address = user;
	else window = SDL_GetGrabbedWindow();
	return AssertMessageBox(message, title, window).Choose();
}

void SetAssertHandler(SDL_Window *window)
{
	SDL_SetAssertionHandler(AssertionHandler, window);
}

void ResetAssertHandler()
{
	SDL_ResetAssertionReport();
	SDL_AssertionHandler handler = SDL_GetDefaultAssertionHandler();
	SDL_SetAssertionHandler(handler, nullptr);
}

signed SDL_perror(const char *origin, const char *error)
{
	SDL_Log("%s: %s", origin, error);
	return 0;
}

signed SDL_perror(const char *origin)
{
	const char *error = SDL_GetError();
	if (error)
	{
		SDL_Log("%s: %s", origin, error);
	}
	return 0;
}
