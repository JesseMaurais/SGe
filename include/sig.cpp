#include "sig.hpp"
#include "err.hpp"
#include "dbg.hpp"
#include <csignal>
#include <cassert>
#include <map>

namespace
{
	sys::sig::subject &event(int signo)
	{
		static std::map<int, sys::sig::subject> map;
		return map[signo];
	}

	void send(int signo)
	{
		event(signo).send([signo](auto const &pair)
		{
			pair.second(signo);
		});
	}
}

namespace sys::sig
{
	handle::handle(int no, sys::sig::observer fn)
	: slot(&::event(no), fn)
	{
		old = std::signal(no, ::send);
		if (SIG_ERR == old)
		{
			sys::ferror("signal", no);
		}
		else
		{
			signo = no;
		}
	}

	handle::~handle()
	{
		verify(::send == std::signal(signo, old));
	}

	handle::operator bool()
	{
		return SIG_ERR != old;
	}
}

