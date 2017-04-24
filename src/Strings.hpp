#ifndef Strings_hpp
#define Strings_hpp

enum Strings
{
	 None
	,Config
	,CommandPrompt
	,CmdHelp
	,CmdConfigs
	,CmdMedia
	,CmdQuit
	,Retry
	,Break
	,Abort
	,Ignore
	,AlwaysIgnore
	,UnknownError
	,NoError
	,OutOfMemory
	,NameNotUnique
	,EncodingError
	,InvalidArgument
	,InvalidName
	,InvalidEnum
	,InvalidValue
	,InvalidOperation
	,InvalidDevice
	,InvalidContext
	,CannotLock
	,CannotUnlock
	,CannotMakeExit
	,CannotFindFormat
	,CannotShowMessageBox
	,CannotQueueEvent
	,CannotCreateContext
	,CannotCreateParser
	,CannotFindSchema
	,CannotLoadScript
	,CannotLoadDocument
	,CannotUseShader
	,CannotUseProgram
	,CannotCreateCommandQueue
	,CaughtException
};

char const *String(enum Strings);
signed SetError(enum Strings);

#endif // file
