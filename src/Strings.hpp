#ifndef Strings_hpp
#define Strings_hpp

enum Strings
{
	 None
	,Application
	,CommandPrompt
	,ColonSeparator
	,NoError
	,UnknownError
	,OutOfMemory
	,EncodingError
	,NoDebugger
	,NameNotUnique
	,CaughtException
	,IniSectionNotUnique
	,IniKeyBeforeSection
	,IniKeyNotUnique
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
	,CannotSendEvent
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
