#ifndef Strings_hpp
#define Strings_hpp

enum Strings
{
	 None
	,Application
	,ColonSeparator
	,Optional
	,Required
	,NoError
	,UnknownError
	,OutOfMemory
	,EncodingError
	,NameNotUnique
	,CaughtException
	,InvalidArgumentRange
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
	,CannotStartProcess
	,CannotFindFormat
	,CannotShowMessageBox
	,CannotQueueEvent
	,CannotCreateContext
	,CannotCreateParser
	,CannotFindSchema
	,CannotLoadScript
	,CannotLoadDocument
	,CannotAllocateResource
	,CannotFreeResource
	,CannotCompileShader
	,CannotQueryShader
	,CannotUseShader
	,CannotQueryValue
	,CannotQueryProgram
	,CannotBuildProgram
	,CannotLinkProgram
	,CannotUseProgram
	,CannotQueryPlatform
	,CannotQueryDevices
};

char const *String(enum Strings);

#endif // file
