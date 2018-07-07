#ifndef Pipe_hpp
#define Pipe_hpp

#include <memory>
#include <string>
#include <vector>

struct SDL_RWops;

namespace SDL
{
	SDL_RWops *Pipe();
	SDL_RWops *Process(std::vector<std::string> const &command);

	class RWops
	{
	public:

		RWops() = default;
		RWops(SDL_RWops *ops);

		void Reset(SDL_RWops *that = nullptr)
		{
			ops.reset(that);
		}

		SDL_RWops *Release()
		{
			return ops.release();
		}

		SDL_RWops *operator->()
		{
			return ops.get();
		}

		operator SDL_RWops*()
		{
			return ops.get();
		}

	private:

		std::unique_ptr<SDL_RWops> ops;
	};
}

namespace sys::io
{
	struct File : SDL::RWops
	{
		bool get(std::string &string, char delim = '\n');
		bool set(std::string const &string);
	};
}

#endif // file
