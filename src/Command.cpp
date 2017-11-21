#include "Command.hpp"
#include "Event.hpp"
#include "SDL.hpp"
#include <cstdio>
#include <vector>
#include <future>
#include <mutex>
#include <condition_variable>

namespace
{
	// For locking with main thread
	std::mutex Mutex;
	std::condition_variable Condition;
	// Flag to signal thread can continue
	bool Ready;

	void Thread()
	{
		std::vector<char> line;
		line.reserve(BUFSIZ);

		while (true)
		{
			if (std::feof(stdin) or std::ferror(stdin))
			{
				break;
			}
			else
			if (not std::fgets(line.data(), line.size(), stdin))
			{
				continue;
			}

			Ready = false; // reset our signal flag
			// SDL_PushEvent is already thread safe
			SDL::SendUserEvent(ExecuteCommand, 0, line.data());
			// Wait for main thread to signal it's done processing
			std::unique_lock<std::mutex> lock(Mutex);
			Condition.wait(lock, []{ return Ready; });
		}
	}
}

bool InitCommand(char const *prompt)
{
	std::async(Thread, prompt);
}

void SignalReady()
{
	std::lock_guard<std::mutex> lock(Mutex);
	Ready = true;
	lock.unlock();
	Condition.notify_one();
}

