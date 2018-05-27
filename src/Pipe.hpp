#ifndef Pipe_hpp
#define Pipe_hpp

struct SDL_RWops;

namespace SDL
{
    SDL_RWops *RWFromProcess(char const *program, char const *mode);
}

namespace sys::io
{
    class Pipe
    {
    public:

        Pipe(SDL_RWops *ops);
        ~Pipe();

        operator SDL_RWops*()
        {
            return ops;
        }

    private:

        SDL_RWops *ops;
    };
}

#endif // file
