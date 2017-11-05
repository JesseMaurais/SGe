#ifndef Strings_hpp
#define Strings_hpp

enum Strings
{
	 None
	,ColonSeparator
	,UnknownError
	,NoError
	,OutOfMemory
	,EncodingError
	,NameNotUnique
	,InvalidArgument
	,InvalidName
	,InvalidEnum
	,InvalidValue
	,InvalidOperation
	,InvalidDevice
	,InvalidContext
	,MessageBoxError
	,MessageBoxWarning
	,MessageBoxInformation
	,MessageBoxContinue
	,MessageBoxAbort
	,MessageBoxRetry
	,MessageBoxBreak
	,MessageBoxIgnore
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
	,CannotCompileShader
	,CannotQueryShader
	,CannotUseShader
	,CannotQueryProgram
	,CannotLinkProgram
	,CannotUseProgram
	,CannotCreateCommandQueue
	,CaughtException
};

char const *String(enum Strings);

#endif // file
