#include <SDL2/SDL_config.h>
#include "Pipe.hpp"
#include "System.hpp"
#include "SDL.hpp"
#include <cstdio>

namespace
{
	auto & PipeR(SDL_RWops *ops)
	{
		return ops->hidden.unknown.data1;
	}

	auto & PipeW(SDL_RWops *ops)
	{
		return ops->hidden.unknown.data2;
	}

	Sint64 Size(SDL_RWops *ops)
	{
		return -1;
	}

	Sint64 Seek(SDL_RWops *ops, Sint64 off, int whence)
	{
		return -1;
	}

	std::size_t Read(SDL_RWops *ops, void *ptr, std::size_t size, std::size_t maxnum)
	{
		return SDL_RWread(PipeR(ops), ptr, size, maxnum);
	}

	std::size_t Write(SDL_RWops *ops, const void *ptr, std::size_t size, std::size_t num)
	{
		return SDL_RWwrite(PipeW(ops), ptr, size, num);
	}

	int Close(SDL_RWops *ops)
	{
		int error = 0;
		if (SDL_RWclose(PipeR(ops)))
		{
			SDL::LogError("PipeR");
			error = -1;
		}
		if (SDL_RWclose(PipeW(ops)))
		{
			SDL::LogError("PipeW");
			error = -1;
		}
		return error;
	}

	SDL_RWops *AllocRW()
	{
		SDL_RWops *ops = SDL_AllocRW();
		if (not ops)
		{
			SDL::SetError(OutOfMemory);
			return nullptr;
		}
		ops->type = SDL_RWOPS_UNKNOWN;
		ops->size = Size;
		ops->seek = Seek;
		ops->read = Read;
		ops->write = Write;
		ops->close = Close;
		return ops;
	}

	SDL_RWops *FromFP(FILE* fp)
	{
		auto ops = SDL_RWFromFP(fp, SDL_TRUE);
		if (not ops)
		{
			SDL::LogError("SDL_RWFromFP");
			if (std::fclose(fp))
			{
				SDL::perror("fclose");
			}
		}
		return ops;
	}

	SDL_RWops *FromFD(int fd, bool readMode)
	{
		auto const mode = readMode ? "r" : "w";
		std::FILE *fp;

		if constexpr (POSIX)
		{
			fp = fdopen(fd, mode);
			if (not fp)
			{
				SDL::perror("fdopen");
				if (close(fd))
				{
					SDL::perror("close");
				}
				return nullptr;
			}
		}
		else
		if constexpr (WIN32)
		{
			fp = _fdopen(fd, mode);
			if (not fp)
			{
				SDL::perror("_fdopen");
				if (_close(fd))
				{
					SDL::perror("_close");
				}
				return nullptr;
			}

		}
		return FromFP(fp);
	}

	template <bool Win32> SDL_RWops *FromHandle(HANDLE h, bool readMode)
	{
		int fd = _open_osfhandle((intptr_t) h, readMode ? _O_RDONLY : _O_WRONLY);
		if (-1 == fd)
		{
			if (not CloseHandle(h[p]))
			{
				Win32::LogLastError("CloseHandle");
			}
			return nullptr
		}
		return FromFD(fd, readMode);
	}

	template <bool Win32> HANDLE ToHandle(SDL_RWops *ops)
	{
		assert(SDL_RWOPS_STDFILE == ops->type);
		int fd = _fileno(ops->hidden.stdio.fp);
		if (-1 == fd)
		{
			SDL::perror("_fileno");
			return nullptr;
		}
		HANDLE h = (HANDLE) _get_osfhandle(fd);
		if (INVALID_HANDLE_VALUE == h)
		{
			SDL::perror("_gets_osfhandle");
			return nullptr;
		}
		return h;
	}
}

namespace SDL
{
	RWops::RWops(SDL_RWops *ops) : ops(std::make_shared(ops, SDL_FreeRW))
	{}

	SDL_RWops *Pipe()
	{
		// Parent handle
		RWops ops = AllocRW();
		if (not ops)
		{
			return nullptr;
		}

		// Child handles
		constexpr int R = 0, W = 1;
		RWops rw[2];

		if constexpr (POSIX)
		{
			int fd[2];
			if (pipe(fd))
			{
				SDL::perror("pipe");
				return nullptr;
			}

			bool ok = true;
			for (int p : {R, W})
			{
				rw[p] = FromFD(fd[p], R == p);
				if (not rw[p])
				{
					ok = false;
					continue;
				}
			}
			if (not ok)
				return nullptr;
		}
		else
		if constexpr (WIN32)
		{
			SECURITY_ATTRIBUTES attr;
			attr.nLength = sizeof(attr);
			attr.bInheritHandle = TRUE;
			attr.lpSecurityDescriptor = NULL;

			HANDLE h[2];
			if (not CreatePipe(h + R, h + W, &attr, 0))
			{
				Win32::LogLastError("CreatePipe");
				return nullptr;
			}

			bool ok = true;
			for (int p : {R, W})
			{
				if (not SetHandleInformation(h[p], HANDLE_FLAG_INHERIT, 0))
				{
					ok = false;
				}

				rw[p] = FromHandle<WIN32>(h[p], R == p);
				if (not rw[p])
				{
					ok = false;
					continue;
				}
			}
			if (not ok)
				return nullptr;
		}

		// Release handles to caller
		
		PipeR(ops) = rw[R].Release();
		PipeW(ops) = rw[W].Release();
		return ops.Release();
	}

	SDL_RWops *Process(char const *command)
	{
		RWops rw[2];
		RWops ops = AllocRW();
		if (not ops)
		{
			return nullptr;
		}
			
		RWops pipe[2];
		bool ok = true;
		for (int p : {0, 1})
		{
			pipe[p] = Pipe();
			if (not pipe[p])
			{
				ok = false;
				continue;
			}
		}
		if (not ok)
			return nullptr;

		if constexpr (POSIX)
		{
			pid_t pid = fork();
			if (pid < 0)
			{
				SDL::perror("fork");
				return nullptr;
			}
			else
			if (0 < pid)
			{
				
			}

			if (dup2(fileno(PipeR(pipe[0])), STDIN_FILENO))
			{
			}

			if (execve(command, nullptr, nullptr))
			{
				SDL::perror("execve");
				return nullptr;
			}
		}
		else
		if constexpr (WIN32)
		{
			STARTUPINFO init;
			ZeroMemory(&init, sizeof(init));
			init.cb = sizeof(init);
			init.hStdInput = ToHandle(PipeW(rw[0]));
			init.hStdOutput = ToHandle(PipeR(rw[1]));
			init.hStdError = GetStdHandle(STD_ERROR_HANDLE);

			// Start the process

			PROCESS_INFORMATION info;
			BOOL const ok = CreateProcess
			(
			 nullptr, // no application name
			 command, // command line
			 nullptr, // process security attributes
			 nullptr, // primary thread security attributes
			 FALSE,   // handles are not inherited
			 0,       // creation flags
			 nullptr, // use parent's environment
			 nullptr, // use parent's current directory
			 &init,   // startup information (in)
			 &info    // process information (out)
			);

			if (not ok)
			{
				Win32::LogLastError("CreateProcess");
				return nullptr;
			}

			// Close unused process handles

			if (not CloseHandle(info.hProcess))
			{
				Win32::LogLastError("CloseHandle(hProcess)");
			}
			if (not CloseHandle(info.hThread))
			{
				Win32::LogLastError("CloseHandle(hThread)");
			}
		}

		// Release handles to caller
			
		PipeW(ops) = rw[0].Release();
		PipeR(ops) = rw[1].Release();
		return ops.Release();
	}
}
