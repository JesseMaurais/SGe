#include "Strings.hpp"
#include "Error.hpp"
#include "SDL.hpp"
#include <string>
#include <sstream>

struct MessageBox : SDL_MessageBoxData
{
	static constexpr unsigned ButtonCount = 5;
	static const SDL_MessageBoxButtonData Buttons[ButtonCount];

	SDL_AssertState Choose(const char *message, const char *title, SDL_Window *window = nullptr)
	{
		this->message = message;
		this->window = window;
		this->title = title;

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

	MessageBox()
	{
		flags = SDL_MESSAGEBOX_ERROR;
		numbuttons = ButtonCount;
		buttons = Buttons;
		colorScheme = &MessageBoxColorScheme;
	}
};

const SDL_MessageBoxButtonData MessageBox::Buttons[MessageBox::ButtonCount] =
{
		{ 0, SDL_ASSERTION_RETRY, ErrorMessageBox::Retry },
		{ 0, SDL_ASSERTION_BREAK, ErrorMessageBox::Break },
		{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, SDL_ASSERTION_ABORT, ErrorMessageBox::Abort },
		{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, SDL_ASSERTION_IGNORE, ErrorMessageBox::Ignore },
		{ 0, SDL_ASSERTION_ALWAYS_IGNORE, ErrorMessageBox::AlwaysIgnore }
};

static SDL_AssertState AssertionHandler(const SDL_AssertData *data, void *user)
{
	if (data->always_ignore)
	{
		return SDL_ASSERTION_IGNORE;
	}

	std::stringstream stream;
	stream << "Assertion failure at " << data->function;
	stream << " (" << data->filename << ":" << data->linenum << "), ";
	const auto quantity = data->trigger_count > 1 ? " times:" : " time:";
	stream << "triggered " << data->trigger_count << quantity << "\n";
	stream << "       '" << data->condition << "'\n";
	std::string string = stream.str();
	const char *title = data->function;
	const char *message = string.c_str();

	union {
		SDL_Window *window;
		void *address;
	};
	MessageBox messageBox;
	if (user) address = user;
	else window = SDL_GetGrabbedWindow();
	return messageBox.Choose(message, title, window);
}

void SetErrorHandler(SDL_Window *window)
{
	SDL_SetAssertionHandler(AssertionHandler, window);
}

void ResetErrorHandler()
{
	SDL_ResetAssertionReport();
	SDL_AssertionHandler handler = SDL_GetDefaultAssertionHandler();
	SDL_SetAssertionHandler(handler, nullptr);
}


