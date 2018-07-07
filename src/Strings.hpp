#ifndef Strings_hpp
#define Strings_hpp

enum Strings
{
	 None
	,CommandPrompt
	,ColonSeparator
	,NoError
	,UnknownError
	,OutOfMemory
	,OutOfRange
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
	,InvalidProgram
	,StartProgram
	,EndProgram
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
	,CannotHandleSignal
	,CannotSendEvent
	,CannotStartProcess
	,CannotFindFormat
	,CannotFindPath
	,CannotFindExecutable
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
