#ifndef Pipe_hpp
#define Pipe_hpp

#include <memory>

struct SDL_RWops;

namespace SDL
{
	SDL_RWops *Pipe();
	SDL_RWops *Process(char const *command, char const *mode);

	class RWops
	{
	public:

		RWops() = default;
		RWops(SDL_RWops *ops);

		SDL_RWops *Release()
		{
			auto p = ops.get();
			ops.reset();
			return p;
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

		std::shared_ptr<SDL_RWops> ops;
	};
}

#endif // file
