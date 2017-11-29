#include "Strings.hpp"
#include <cassert>

char const *String(enum Strings id)
{
	switch (id)
	{
	case None: return "";
	case Application: return "SGe";
	case CommandPrompt: return "> ";
	case ColonSeparator: return "%1: %2";
	case NoError: return "Error reported without information";
	case UnknownError: return "Unknown error";
	case OutOfMemory: return "Out of memory";
	case EncodingError: return "Encoding error";
	case NoDebugger: return "There is no debugger";
	case NameNotUnique: return "The name \"%1\" is not unique";
	case CaughtException: return "C++ exception was caught";
	case IniSectionNotUnique: return "Section %1 was repeated";
	case IniKeyBeforeSection: return "Key %1 appears before a section.";
	case IniKeyNotUnique: return "Key %1 was repeated";
	case InvalidArgumentRange: return "Invalid argument range";
	case InvalidArgument: return "Invalid argument \"%1\" to function";
	case InvalidName: return "Invalid name";
	case InvalidEnum: return "Invalid enumeration";
	case InvalidValue: return "Invalid value";
	case InvalidOperation: return "Invalid operation";
	case InvalidDevice: return "Invalid device";
	case InvalidContext: return "Invalid context";
	case MessageBoxError: return "Error";
	case MessageBoxWarning: return "Warning";
	case MessageBoxInformation: return "Information";
	case MessageBoxContinue: return "Continue";
	case MessageBoxAbort: return "Abort";
	case MessageBoxRetry: return "Retry";
	case MessageBoxBreak: return "Break";
	case MessageBoxIgnore: return "Ignore";
	case CannotLock: return "Cannot lock";
	case CannotUnlock: return "Cannot unlock";
	case CannotMakeExit: return "Cannot make %1 at exit";
	case CannotStartProcess: return "Cannot start a system process";
	case CannotFindFormat: return "Cannot find matching format";
	case CannotShowMessageBox: return "Cannot show message box";
	case CannotQueueEvent: return "Cannot queue event";
	case CannotCreateContext: return "Cannot create context";
	case CannotCreateParser: return "Cannot create parser";
	case CannotFindSchema: return "Cannot find \"%1\" in schema";
	case CannotLoadScript: return "Cannot load script \"%1\"";
	case CannotLoadDocument: return "Cannot load document \"%1\"";
	case CannotAllocateResource: return "Cannot allocate resource";
	case CannotFreeResource: return "Cannot free resource";
	case CannotCompileShader: return "Cannot compile shader";
	case CannotQueryShader: return "Cannot query shader value";
	case CannotUseShader: return "Cannot use shader for program";
	case CannotQueryValue: return "Cannot query a value";
	case CannotQueryProgram: return "Cannot query shader program value";
	case CannotBuildProgram: return "Cannot build program";
	case CannotLinkProgram: return "Cannot link program";
	case CannotUseProgram: return "Cannot use program";
	case CannotQueryPlatform: return "Cannot query platform";
	case CannotQueryDevices: return "Cannot query devices";
	}
	assert(not "Valid strings id");
	return nullptr;
}
