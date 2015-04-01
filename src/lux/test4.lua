print 'test4: exporting common UNIX system functions with C++11 lambda'

-- Who are you?

print('Your login name is ' .. uni.getlogin())

-- Using a char array to obtain string data from the system

path = char(1000)
uni.getcwd(path, #path)
print('The current working directory is ' .. path)
uni.confstr(uni._CS_PATH, path, #path)
print('The system PATH environment is ' .. path)

-- Find some conformance features of the POSIX environoment

print('This system conforms to POSIX version ' .. uni.sysconf(uni._SC_VERSION))
print('This system conforms to XOPEN version ' .. uni._XOPEN_VERSION)
if uni._XOPEN_UNIX or uni.sysconf(uni._SC_XOPEN_UNIX) > 0 then
	print('This system supports the XSI extension')
end
if uni._BOGUS_UNIX_CONFIG then
	print('You should never read this')
end

-- A legacy function

print('The memory page size of your machine is ' .. uni.getpagesize())

-- UNIX pipes

print('The following message is sent through a pipe.')

msg = char('\tHello World!\n')
uni.write(uni.STDOUT_FILENO, msg, #msg)

-- ... perchance to dream

print('Now we will sleep for 2 seconds')
uni.sleep(2)
print('Now we awake')

print('Now we die')
uni._exit(0)
print("Now we're dead") -- We don't get here

