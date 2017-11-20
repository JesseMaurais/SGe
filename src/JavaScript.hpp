#ifndef JavaScript_hpp
#define JavaScript_hpp

#include <string>
#include <vector>
#include <cstdint>
#include <jerryscript.h>

namespace js
{
	bool Init(jerry_init_flag_t const flags);

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

	template <typename Context> class ContextManager : jerry_context_data_manager_t
	{
		static ContextManager &Instance()
		{
			static ContextManager singleton;
			return singleton;
		}

		operator Context*()
		{
			return static_cast<Context*>(jerry_get_context_data(this));
		}

	private:

		ContextManager()
		{
			init_cb = Construct;
			deinit_cb = Destruct;
		}

		static void Construct(void *data)
		{
			new (data) Context;
		}

		static void Destruct(void *data)
		{
			union { void *address; Context *object; };
			address = data;
			object->~Context();
		}
	};
}

#endif // file
