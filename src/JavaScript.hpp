#ifndef JavaScript_hpp
#define JavaScript_hpp

#include <string>
#include <vector>
#include <cstdint>
#include <jerryscript.h>

namespace js
{
	/// Initialize the JavaScript engine
	bool Init(jerry_init_flag_t const flags);
	/// Set the SDL error string to the given error code. Always returns true
	bool SetError(jerry_value_t const value);
	/// Set the SDL error string to the current error code. Returns true when an error exists
	bool CheckError(jerry_value_t const value);

	using Snapshot = std::vector<std::uint32_t>;

	enum Parse { None=0, Global=1, Strict=2 };
	bool SaveSnapshot(std::string const &source, Snapshot &buffer, enum Parse opt = None);

	constexpr bool CopyBytecode = true;
	bool ExecuteSnapshot(Snapshot const &shapshot, bool copyBytecode = false);

	/// Utility class to automate release of values according to RAII
	class ScopedValue
	{
	public:

		operator jerry_value_t() const
		{
			return value;
		}

		ScopedValue(jerry_value_t const release)
		{
			value = release;
		}

		~ScopedValue()
		{
			jerry_release_value(value);
		}

	private:

		jerry_value_t value;
	};
}

#endif // file
