#ifndef Strings_hpp
#define Strings_hpp

constexpr auto ConfigScript = "init.lua";
constexpr auto CommandPrompt = ">> ";

namespace ErrorMessageBox
{
constexpr auto Retry = "Retry";
constexpr auto Break = "Break";
constexpr auto Abort = "Abort";
constexpr auto Ignore = "Ignore";
constexpr auto AlwaysIgnore = "Always Ignore";
}

namespace CommandLine
{
constexpr auto Unknown = "Unknown command %s";
constexpr auto Help = "help";
constexpr auto Configs = "init";
constexpr auto Media = "media";
constexpr auto Quit = "quit";
}

constexpr auto UnknownError = "Unknown error";
constexpr auto NoError = "Error reported without information";
constexpr auto OutOfMemory = "Out of memory";
constexpr auto BadArgument = "Bad argument \"%s\" to function";
constexpr auto InvalidName = "Invalid name";
constexpr auto InvalidEnum = "Invalid enumeration";
constexpr auto InvalidValue = "Invalid value";
constexpr auto InvalidOperation = "Invalid operation";
constexpr auto InvalidDevice = "Invalid device";
constexpr auto InvalidContext = "Invalid context";
constexpr auto CannotLock = "Cannot lock";
constexpr auto CannotUnlock = "Cannot unlock";
constexpr auto CannotMakeExit = "Cannot make %s at exit";
constexpr auto CannotFindFormat = "Cannot find matching format";
constexpr auto CannotShowMessageBox = "Cannot show message box";
constexpr auto CannotQueueEvent = "Cannot queue event";
constexpr auto CannotCreateContext = "Cannot create context";
constexpr auto CannotCreateParser = "Cannot create parser";
constexpr auto CannotFindSchema = "Cannot find \"%s\" in schema";
constexpr auto CannotLoadScript = "Cannot load script \"%s\"";
constexpr auto CannotLoadDocument = "Cannot load document \"%s\"";
constexpr auto CannotUseShader = "Cannot use shader";
constexpr auto CannotUseShaderProgram = "Cannot use shader program";
constexpr auto CannotCreateCommandQueue = "Cannot create command queue";
constexpr auto CaughtException = "C++ exception: %s";
constexpr auto NameNotUnique = "The name \"%s\" is not unique";
constexpr auto EncodingError = "Encoding error";

#endif // file
