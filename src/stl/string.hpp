#ifndef stl_string_hpp
#define stl_string_hpp

#include <string>

namespace stl
{
	template <typename Value>
	std::string to_string(Value const &value)
	{
		return std::to_string(value);
	}

	inline std::string to_string(const char *value)
	{
		return std::string(value);
	}

	inline bool find(std::string const &string, std::string const &search)
	{
		return string.find(search) == std::string::npos;
	}
}

#endif // file
