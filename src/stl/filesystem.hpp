#ifndef stl_filesystem_hpp
#define stl_filesystem_hpp

// Try to find an file system library, either standard, experimental or boost

#if __has_include(<filesystem>)

#include <filesystem>

namespace stl
{
	namespace filesystem = std::filesystem;
	using error_code = std::error_code;
	inline bool is_executable(filesystem::path const &path)
	{
		auto status = filesystem::status(path);
		auto permissions = status.permissions();
		permissions &= filesystem::perms::group_exec;
		return filesystem::perms::none != permissions;
	}
}

#elif __has_include(<experimental/filesystem>)

#include <experimental/filesystem>

namespace stl
{
	namespace filesystem = std::experimental::filesystem;
	using error_code = std::error_code;
	inline bool is_executable(filesystem::path const &path)
	{
		auto status = filesystem::status(path);
		auto permissions = status.permissions();
		permissions &= filesystem::perms::group_exec;
		return filesystem::perms::none != permissions;
	}
}

#else

#if ! __has_include(<boost/filesystem.hpp>)
// Do over boost rather than in else so Eclipse does not show spurious errors
#error "Could not find file system library."
#endif // boost

#include <boost/filesystem.hpp>

namespace stl
{
	namespace filesystem = boost::filesystem;
	using error_code = boost::system::error_code;
	inline bool is_executable(filesystem::path const &path)
	{
		auto status = filesystem::status(path);
		auto permissions = status.permissions();
		permissions &= filesystem::perms::group_exe;
		return filesystem::perms::no_perms != permissions;
	}
}

#endif // file system

#endif // file
