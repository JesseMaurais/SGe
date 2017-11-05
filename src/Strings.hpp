#ifndef Strings_hpp
#define Strings_hpp

enum Strings
{
	 None
	,ColonSeparator
	,NoError
	,UnknownError
	,OutOfMemory
	,EncodingError
	,NameNotUnique
	,CaughtException
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
	,CannotCreateResource
	,CannotDeleteResource
	,CannotCompileShader
	,CannotQueryShader
	,CannotUseShader
	,CannotQueryProgram
	,CannotLinkProgram
	,CannotUseProgram
};

char const *String(enum Strings);

#endif // file
