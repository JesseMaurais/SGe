#include "Strings.hpp"
#include "SDL.hpp"
#include <cassert>

char const *String(enum Strings id)
{
	switch (id)
	{
	default:
		assert(not "Valid strings id");
		return nullptr;

	case Strings::None: return "";
	case Strings::Application: return "SGe";
	case Strings::ColonSeparator: return "%1: %2";
	case Strings::NoError: return "Error reported without information";
	case Strings::UnknownError: return "Unknown error";
	case Strings::OutOfMemory: return "Out of memory";
	case Strings::EncodingError: return "Encoding error";
	case Strings::NameNotUnique: return "The name \"%1\" is not unique";
	case Strings::CaughtException: return "C++ exception was caught";
	case Strings::InvalidArgument: return "Invalid argument \"%1\" to function";
	case Strings::InvalidName: return "Invalid name";
	case Strings::InvalidEnum: return "Invalid enumeration";
	case Strings::InvalidValue: return "Invalid value";
	case Strings::InvalidOperation: return "Invalid operation";
	case Strings::InvalidDevice: return "Invalid device";
	case Strings::InvalidContext: return "Invalid context";
	case Strings::MessageBoxError: return "Error";
	case Strings::MessageBoxWarning: return "Warning";
	case Strings::MessageBoxInformation: return "Information";
	case Strings::MessageBoxContinue: return "Continue";
	case Strings::MessageBoxAbort: return "Abort";
	case Strings::MessageBoxRetry: return "Retry";
	case Strings::MessageBoxBreak: return "Break";
	case Strings::MessageBoxIgnore: return "Ignore";
	case Strings::CannotLock: return "Cannot lock";
	case Strings::CannotUnlock: return "Cannot unlock";
	case Strings::CannotMakeExit: return "Cannot make %1 at exit";
	case Strings::CannotStartProcess: return "Cannot start a system process";
	case Strings::CannotFindFormat: return "Cannot find matching format";
	case Strings::CannotShowMessageBox: return "Cannot show message box";
	case Strings::CannotQueueEvent: return "Cannot queue event";
	case Strings::CannotCreateContext: return "Cannot create context";
	case Strings::CannotCreateParser: return "Cannot create parser";
	case Strings::CannotFindSchema: return "Cannot find \"%1\" in schema";
	case Strings::CannotLoadScript: return "Cannot load script \"%1\"";
	case Strings::CannotLoadDocument: return "Cannot load document \"%1\"";
	case Strings::CannotAllocateResource: return "Cannot allocate resource";
	case Strings::CannotFreeResource: return "Cannot free resource";
	case Strings::CannotCompileShader: return "Cannot compile shader";
	case Strings::CannotQueryShader: return "Cannot query shader value";
	case Strings::CannotUseShader: return "Cannot use shader for program";
	case Strings::CannotQueryValue: return "Cannot query a value";
	case Strings::CannotQueryProgram: return "Cannot query shader program value";
	case Strings::CannotBuildProgram: return "Cannot build program";
	case Strings::CannotLinkProgram: return "Cannot link program";
	case Strings::CannotUseProgram: return "Cannot use program";
	case Strings::CannotQueryPlatform: return "Cannot query platform";
	case Strings::CannotQueryDevices: return "Cannot query devices";
	}
}
