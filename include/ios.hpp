#ifndef ios_hpp
#define ios_hpp

#include <ios>

namespace io
{
	using openmode = std::ios_base::openmode;
	enum
	{
		app   = std::ios_base::app,
		bin   = std::ios_base::binary,
		in    = std::ios_base::in,
		out   = std::ios_base::out,
		trunc = std::ios_base::trunc,
		ate   = std::ios_base::ate,
	};
}

#endif // file
