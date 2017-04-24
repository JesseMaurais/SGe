#include "Strings.hpp"
#include "SDL.hpp"

char const *String(enum Strings id)
{
	switch (id)
	{
	case Strings::None: return "";
	case Strings::CommandPrompt: return ">>";
	case Strings::Config: return "init.lua";
	case Strings::CmdHelp: return "Help";
	case Strings::CmdConfigs: return "Configs";
	case Strings::CmdMedia: return "Media";
	case Strings::CmdQuit: return "Quit";
	case Strings::Retry: return "Retry";
	case Strings::Break: return "Break";
	case Strings::Abort: return "Abort";
	case Strings::Ignore: return "Ignore";
	case Strings::AlwaysIgnore: return "Always Ignore";
	case Strings::UnknownError: return "Unknown error";
	case Strings::NoError: return "Error reported without information";
	case Strings::OutOfMemory: return "Out of memory";
	case Strings::InvalidArgument: return "Invalid argument \"%1\" to function";
	case Strings::InvalidName: return "Invalid name";
	case Strings::InvalidEnum: return "Invalid enumeration";
	case Strings::InvalidValue: return "Invalid value";
	case Strings::InvalidOperation: return "Invalid operation";
	case Strings::InvalidDevice: return "Invalid device";
	case Strings::InvalidContext: return "Invalid context";
	case Strings::CannotLock: return "Cannot lock";
	case Strings::CannotUnlock: return "Cannot unlock";
	case Strings::CannotMakeExit: return "Cannot make %1 at exit";
	case Strings::CannotFindFormat: return "Cannot find matching format";
	case Strings::CannotShowMessageBox: return "Cannot show message box";
	case Strings::CannotQueueEvent: return "Cannot queue event";
	case Strings::CannotCreateContext: return "Cannot create context";
	case Strings::CannotCreateParser: return "Cannot create parser";
	case Strings::CannotFindSchema: return "Cannot find \"%1\" in schema";
	case Strings::CannotLoadScript: return "Cannot load script \"%1\"";
	case Strings::CannotLoadDocument: return "Cannot load document \"%1\"";
	case Strings::CannotUseShader: return "Cannot use shader";
	case Strings::CannotUseProgram: return "Cannot use shader program";
	case Strings::CannotCreateCommandQueue: return "Cannot create command queue";
	case Strings::CaughtException: return "C++ exception: %1";
	case Strings::NameNotUnique: return "The name \"%1\" is not unique";
	case Strings::EncodingError: return "Encoding error";
	}
	return "STRING MISSING";
}

signed SetError(enum Strings id)
{
	return SDL_SetError(String(id));
}
