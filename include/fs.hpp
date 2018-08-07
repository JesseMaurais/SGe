#ifndef fs_hpp
#define fs_hpp

// Try to find an file system library, either standard, experimental or boost

#if __has_include(<filesystem>)

#include <filesystem>

namespace sys::file
{
	using namespace std::filesystem;
	using error_code = std::error_code;
	inline bool is_executable(path const &exe)
	{
		return perms::none != (status(exe).permissions() & perms::group_exec);
	}
}

#elif __has_include(<experimental/filesystem>)

#include <experimental/filesystem>

namespace sys::file
{
	using namespace std::experimental::filesystem;
	using error_code = std::error_code;
	inline bool is_executable(path const &exe)
	{
		return perms::none != (status(exe).permissions() & perms::group_exec);
	}
}

#else

#if ! __has_include(<boost/filesystem.hpp>)
// Do over boost rather than in else so Eclipse does not show spurious errors
#error "Could not find file system library."
#endif // boost

#include <boost/filesystem.hpp>

namespace sys::file
{
	using namespace boost::filesystem;
	using error_code = boost::system::error_code;
	inline bool is_executable(path const &exe)
	{
		return perms::no_perms != (status(exe).permissions() & perms::group_exe);
	}
}

#endif // file system

#include <fstream>

namespace sys::file
{
	inline bool append(path name, std::string_view text)
	{
		std::ofstream stream(name, std::ios::app);
		return stream and stream << text;
	}

	inline bool truncate(path name)
	{
		std::ofstream stream(name, std::ios::trunc);
		return stream.good();
	}
}

#endif // file
