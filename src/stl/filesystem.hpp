#ifndef stl_filesystem_hpp
#define stl_filesystem_hpp

// Try to find an filesystem library, either C++17 or Boost

#if __has_include(<filesystem>)
#include <filesystem>
namespace stl
{
	namespace filesystem = std::filesystem;
}
#else
#if __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace stl
{
	namespace filesystem = std::experimental::filesystem;
}
#else
#if __has_include(<boost/filesystem.hpp>)
#include <boost/filesystem.hpp>
namespace stl
{
	namespace filesystem = boost::filesystem;
}
#endif // boost
#endif // experimental
#endif // filesystem

#endif // file
