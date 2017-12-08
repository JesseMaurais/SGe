#include "Stream.hpp"
#include "Event.hpp"
#include "SDL.hpp"
#include <cstdio>
#include <vector>
#include <future>
#include <mutex>
#include <condition_variable>
#include <readline/readline.h>
#include <readline/history.h>

namespace
{
	// For locking with main thread
	std::mutex Mutex;
	std::condition_variable Condition;
	// Flag to signal thread can continue
	bool Ready;

	void SendStream(bool strict, char *line, std::size_t size)
	{
		Ready = false; // reset signal flag
		std::string string(line, size);
		// SDL_PushEvent is already thread safe
		SDL::SendUserEvent(EvaluateScript, strict, string);
		// Wait for main thread to signal it's done processing
		std::unique_lock<std::mutex> lock(Mutex);
		Condition.wait(lock, []{ return Ready; });
	}

	void Readfile(std::FILE *input, std::size_t buffer, bool strict)
	{
		std::vector<char> line(buffer);

		while (true)
		{
			if (std::feof(input) or std::ferror(input))
			{
				break;
			}
			else
			if (not std::fgets(line.data(), line.size(), input))
			{
				continue;
			}
			SendStream(strict, line.data(), line.size());
		}
	}

	void Readline(std::string prompt, bool strict)
	{
		char const *string = prompt.empty() ? nullptr : prompt.c_str();
		while (true)
		{
			auto line = readline(string);
			if (not line)
			{
				break;
			}
			add_history(line);
			std::size_t size = std::strlen(line);
			SendStream(strict, line, size);
			std::free(line);
		}
	}
}

bool InitStream(std::string const &prompt, std::FILE *file, std::size_t buffer, bool strict)
{
	if (not file)
	{
		static auto future = std::async(Readline, prompt, strict);
		return future.valid();
	}
	else
	{
		static auto future = std::async(Readfile, file, buffer, strict);
		return future.valid();
	}
	return false;
}

void SignalStream()
{
	std::lock_guard<std::mutex> lock(Mutex);
	Ready = true;
	Mutex.unlock();
	Condition.notify_one();
}

