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

	void SendStream(bool strict, char *data, std::size_t size)
	{
		Ready = false; // reset signal flag
		// SDL_PushEvent is already thread safe
		SDL::SendUserEvent(EvaluateScript, strict, data, data + size);
		// Wait for main thread to signal it's done processing
		std::unique_lock<std::mutex> lock(Mutex);
		Condition.wait(lock, []{ return Ready; });
	}

	void Thread(std::FILE *input, std::size_t buffer, bool strict)
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
		while (true)
		{
			auto line = readline(prompt.c_str());
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

bool InitStream(std::FILE *file, std::string prompt, std::size_t buffer, bool strict)
{
	if (not prompt.empty())
	{
		std::async(Readline, prompt, strict);
	}
	else
	{
		std::async(Thread, file, buffer, strict);
	}
	return true;
}

void SignalStream()
{
	std::lock_guard<std::mutex> lock(Mutex);
	Ready = true;
	Mutex.unlock();
	Condition.notify_one();
}

