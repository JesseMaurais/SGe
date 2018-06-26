#include <SDL2/SDL_config.h>
#include <cstdio>
#include "Pipe.hpp"
#include "System.hpp"
#include "SDL.hpp"

namespace
{
	constexpr int R = 0, W = 1;

	auto & dataR(SDL_RWops *ops)
	{
		return ops->hidden.unknown.data1;
	}

	auto & dataW(SDL_RWops *ops)
	{
		return ops->hidden.unknown.data2;
	}

	SDL_RWops *Rops(SDL_RWops *ops)
	{
		return (SDL_RWops*) dataR(ops);
	}

	SDL_RWops *Wops(SDL_RWops *ops)
	{
		return (SDL_RWops*) dataW(ops);
	}

	std::FILE *fileR(SDL_RWops *ops)
	{
		assert(SDL_RWOPS_STDFILE == ops->type);
		return Rops(ops)->hidden.stdio.fp;
	}

	std::FILE *fileW(SDL_RWops *ops)
	{
		assert(SDL_RWOPS_STDFILE == ops->type);
		return Wops(ops)->hidden.stdio.fp;
	}

	Sint64 Size(SDL_RWops *ops)
	{
		SDL::SetError(std::errc::not_supported);
		return -1;
	}

	Sint64 Seek(SDL_RWops *ops, Sint64 off, int whence)
	{
		SDL::SetError(std::errc::not_supported);
		return -1;
	}

	std::size_t Read(SDL_RWops *ops, void *ptr, std::size_t size, std::size_t maxnum)
	{
		return SDL_RWread(Rops(ops), ptr, size, maxnum);
	}

	std::size_t Write(SDL_RWops *ops, const void *ptr, std::size_t size, std::size_t num)
	{
		return SDL_RWwrite(Wops(ops), ptr, size, num);
	}

	int Close(SDL_RWops *ops)
	{
		int error = 0;
		if (not Rops(ops) or SDL_RWclose(Rops(ops)))
		{
			SDL::LogError("SDL_RWclose(R)");
			error = -1;
		}
		if (not Wops(ops) or SDL_RWclose(Wops(ops)))
		{
			SDL::LogError("SDL_RWclose(W)");
			error = -1;
		}
		return error;
	}

	SDL_RWops *AllocRW()
	{
		SDL_RWops *ops = SDL_AllocRW();
		if (not ops)
		{
			SDL::SetError(std::errc::not_enough_memory);
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

	SDL_RWops *FromFP(std::FILE* fp)
	{
		SDL_RWops *ops = SDL_RWFromFP(fp, SDL_TRUE);
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

#ifdef __WIN32__

	SDL_RWops *FromHandle(HANDLE h, bool readMode)
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

	HANDLE ToHandle(SDL_RWops *ops)
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

#endif // __WIN32__
}

namespace SDL
{
	RWops::RWops(SDL_RWops *ops) : ops(std::make_shared(ops, SDL_FreeRW))
	{}

	SDL_RWops *Pipe()
	{
		RWops ops = AllocRW();
		if (not ops)
		{
			return nullptr;
		}

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

				rw[p] = FromHandle(h[p], R == p);
				if (not rw[p])
				{
					ok = false;
					continue;
				}
			}
			if (not ok)
				return nullptr;
		}

		// Release to caller
		dataR(ops) = rw[R].Release();
		dataW(ops) = rw[W].Release();
		return ops.Release();
	}

	SDL_RWops *Process(std::vector<std::string> const &args)
	{
		RWops ops = AllocRW();
		if (not ops)
		{
			return nullptr;
		}
		
		RWops rw[2];
		bool ok = true;
		for (int p : {R, W})
		{
			rw[p] = Pipe();
			if (not rw[p])
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
			if (-1 == pid)
			{
				SDL::perror("fork");
				return nullptr;
			}
			else
			if (0 == pid) // child
			{
				if (-1 == dup2(fileno(fileR(rw[R])), STDIN_FILENO))
				{
					SDL::perror("dup2(R)");
					std::exit(std::errno):
				}

				if (-1 == dup2(fileno(fileW(rw[W])), STDOUT_FILENO))
				{
					SDL::perror("dup2(W)");
					std::exit(std::errno);
				}

				for (int p : {R, W}) rw[p].Reset();

				if (execl(command, command))
				{
					SDL::perror("execv");
					std::exit(std::errno);
				}
			}
		}
		else
		if constexpr (WIN32)
		{
			STARTUPINFO init;
			ZeroMemory(&init, sizeof(init));
			init.cb = sizeof(init);
			init.hStdInput = ToHandle(Rops(rw[R]));
			init.hStdOutput = ToHandle(Wops(rw[W]));
			init.hStdError = GetStdHandle(STD_ERROR_HANDLE);

			PROCESS_INFORMATION info;
			BOOL const ok = CreateProcess
			(
			 NULL,    // no application name
			 command, // command line
			 NULL,    // process security attributes
			 NULL,    // primary thread security attributes
			 FALSE,   // handles are not inherited
			 0,       // creation flags
			 NULL,    // use parent's environment
			 NULL,    // use parent's current directory
			 &init,   // startup information (in)
			 &info    // process information (out)
			);

			if (not ok)
			{
				Win32::LogLastError("CreateProcess");
				return nullptr;
			}

			if (not CloseHandle(info.hProcess))
			{
				Win32::LogLastError("CloseHandle(hProcess)");
			}
			if (not CloseHandle(info.hThread))
			{
				Win32::LogLastError("CloseHandle(hThread)");
			}
		}
	
		SDL_RWops *raw[2];
		for (int p : {R, W}) raw[p] = rw[p].Release();

		// Delete with RAII
		rw[R] = Rops(raw[R]);
		rw[W] = Wops(raw[W]);

		// Release to caller
		dataR(ops) = dataR(raw[W]);
		dataW(ops) = dataW(raw[R]);
		return ops.Release();
	}
}
