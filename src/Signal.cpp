#include "Signal.hpp"
#include "Error.hpp"
#include <csignal>
#include <map>

namespace
{
    sys::sig::Slot::Subject &SubjectMap(int signo)
    {
        static std::map<int, sys::sig::Slot::Subject> map;
        return map[signo];
    }

    void SignalHandler(int signo)
    {
        SubjectMap(signo).Emit([signo](auto const &pair)
        {
            pair.second(signo);
        });
    }
}

namespace sys::sig
{
    ScopedHandler::ScopedHandler(int signo, Observer handler)
    : Slot(SubjectMap(signo), handler)
    {
        this->signo = signo;
        this->handler = std::signal(signo, SignalHandler);
        if (SIG_ERR == this->handler)
        {
            SDL::SetErrno();
        }
    }

    ScopedHandler::~ScopedHandler()
    {
        handler = std::signal(signo, handler);
        assert(SignalHandler == handler);
    }

    ScopedHandler::operator bool()
    {
        return SIG_ERR == handler;
    }
}
