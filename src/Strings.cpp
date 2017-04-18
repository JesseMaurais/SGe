#include "Strings.hpp"

char const *String(enum Strings id)
{
	switch (id)
	{
	case String::None: return "";
	case String::Retry: return "Retry";
	case String::Break: return "Break";
	case String::Abort: return "Abort";
	case String::Ignore: return "Ignore";
	case String::AlwaysIgnore: return "Always Ignore";
	case String::UnknownError: return "Unknown error";
	case String::NoError: return "Error reported without information";
	case String::OutOfMemory: return "Out of memory";
	case String::InvalidArgument: return "Invalid argument \"%1\" to function";
	case String::InvalidName: return "Invalid name";
	case String::InvalidEnum: return "Invalid enumeration";
	case String::InvalidValue: return "Invalid value";
	case String::InvalidOperation: return "Invalid operation";
	case String::InvalidDevice: return "Invalid device";
	case String::InvalidContext: return "Invalid context";
	case String::CannotLock: return "Cannot lock";
	case String::CannotUnlock: return "Cannot unlock";
	case String::CannotMakeExit: return "Cannot make %1 at exit";
	case String::CannotFindFormat: return "Cannot find matching format";
	case String::CannotShowMessageBox: return "Cannot show message box";
	case String::CannotQueueEvent: return "Cannot queue event";
	case String::CannotCreateContext: return "Cannot create context";
	case String::CannotCreateParser: return "Cannot create parser";
	case String::CannotFindSchema: return "Cannot find \"%1\" in schema";
	case String::CannotLoadScript: return "Cannot load script \"%1\"";
	case String::CannotLoadDocument: return "Cannot load document \"%1\"";
	case String::CannotUseShader: return "Cannot use shader";
	case String::CannotUseProgram: return "Cannot use shader program";
	case String::CannotCreateCommandQueue: return "Cannot create command queue";
	case String::CaughtException: return "C++ exception: %1";
	case String::NameNotUnique: return "The name \"%1\" is not unique";
	case String::EncodingError: return "Encoding error";
	}
	return "STRING MISSING";
}
