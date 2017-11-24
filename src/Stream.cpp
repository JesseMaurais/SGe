#include "Stream.hpp"
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

	void Thread(std::FILE *file, std::size_t buffer, bool strict)
	{
		std::vector<char> line(buffer);

		while (true)
		{
			if (std::feof(file) or std::ferror(file))
			{
				break;
			}
			else
			if (not std::fgets(line.data(), line.size(), file))
			{
				continue;
			}

			Ready = false; // reset signal flag
			// SDL_PushEvent is already thread safe
			SDL::SendUserEvent(EvaluateScript, strict, line.data(), line.data() + line.size());
			// Wait for main thread to signal it's done processing
			std::unique_lock<std::mutex> lock(Mutex);
			Condition.wait(lock, []{ return Ready; });
		}
	}
}

bool InitStream(std::FILE *file, std::size_t buffer, bool strict)
{
	std::async(Thread, file, buffer, strict);
	return true;
}

void SignalStream()
{
	std::lock_guard<std::mutex> lock(Mutex);
	Ready = true;
	Mutex.unlock();
	Condition.notify_one();
}

