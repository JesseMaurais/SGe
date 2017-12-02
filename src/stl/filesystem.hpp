#ifndef stl_filesystem_hpp
#define stl_filesystem_hpp

// Try to find an filesystem library, either C++17 or Boost
#if __has_include(<filesystem>)
#include <filesystem>
namespace stl
{
	namespace filesystem = std::filesystem;
	using error_code = std::error_code;
	inline bool is_exe(filesystem::path const &path)
	{
		auto status = filesystem::status(path);
		auto permissions = status.permissions();
		permissions &= filesystem::perms::group_exec;
		return filesystem::perms::none != permissions;
	}
}
#else
#if __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace stl
{
	namespace filesystem = std::experimental::filesystem;
	using error_code = std::error_code;
	inline bool is_exe(filesystem::path const &path)
	{
		auto status = filesystem::status(path);
		auto permissions = status.permissions();
		permissions &= filesystem::perms::group_exec;
		return filesystem::perms::none != permissions;
	}
}
#else
#if __has_include(<boost/filesystem.hpp>)*/
#include <boost/filesystem.hpp>
namespace stl
{
	namespace filesystem = boost::filesystem;
	using error_code = boost::system::error_code;
	inline bool is_exe(filesystem::path const &path)
	{
		auto status = filesystem::status(path);
		auto permissions = status.permissions();
		permissions &= filesystem::perms::group_exe;
		return filesystem::perms::no_perms != permissions;
	}
}
#else
#error "Could not find a file system library"
#endif // boost
#endif // experimental
#endif // filesystem

#endif // file
