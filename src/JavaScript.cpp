#include "JavaScript.hpp"
#include "Error.hpp"
#include "std.hpp"

#include <jerryscript.h>

namespace
{
	// Native object information utility template

	template <typename Pointer> class NativeInfo : jerry_object_native_info_t
	{
	public:

		static NativeInfo &Instance()
		{
			static NativeInfo singleton;
			return singleton;
		}

		jerry_value_t CreateObject(Pointer object)
		{
			jerry_value_t const value = jerry_create_object();
			SetObject(value, object);
			return value;
		}

		void SetObject(jerry_value_t const obj, Pointer object)
		{
			Convert cast;
			cast.object = object;
			jerry_set_object_native_pointer(obj, cast.address, this);
		}

		Pointer GetObject(jerry_value_t const obj)
		{
			Convert cast;
			jerry_object_native_info_t const *info;
			if (jerry_get_object_native_pointer(obj, &cast.address, &info))
			{
				if (info == this)
				{
					return cast.object;
				}
			}
			return nullptr;
		}

		bool IsObject(jerry_value_t const obj)
		{
			return GetObject(obj) != nullptr;
		}

	private:

		union Convert
		{
			void *address;
			Pointer object;
		};

		static void Free(void *native)
		{
			Convert cast;
			cast.address = native;
			delete cast.object;
		}

		NativeInfo()
		{
			free_cb = Free;
		}
	};

	// Native object information for functions

	template <typename Object, typename Result, typename... Args>
	class NativeInfo<Result(Object::*)(Args...)> : jerry_object_native_info_t
	{
	public:

		using Signature = Result(Object::*)(Args...);

		static jerry_value_t Handler(
		       jerry_value_t const value,
			   jerry_value_t const object,
			   jerry_value_t const argv[],
			   jerry_length_t const argc);

		static NativeInfo &Instance()
		{
			static NativeInfo singleton;
			return singleton;
		}

		jerry_value_t CreateObject(Signature function)
		{
			jerry_value_t const value = jerry_create_external_function(Handler);
			SetObject(value, function);
			return value;
		}

		void SetObject(jerry_value_t const obj, Signature function)
		{
			Convert cast;
			cast.function = function;
			jerry_set_object_native_pointer(obj, cast.address, this);
		}

		Signature GetObject(jerry_value_t const obj)
		{
			Convert cast;
			jerry_object_native_info_t const *info;
			if (jerry_get_object_native_pointer(obj, &cast.address, &info))
			{
				if (info == this)
				{
					return cast.function;
				}
			}
			return nullptr;
		}

		bool IsObject(jerry_value_t const obj)
		{
			return GetObject(obj) != nullptr;
		}

	private:

		union Convert
		{
			void *address;
			Signature function;
		};

		NativeInfo()
		{
			free_cb = nullptr;
		}
	};

	// Simplified syntax of for the native object singleton

	template <typename Object> jerry_value_t CreateObject(Object object)
	{
		return NativeInfo<Object>::Instance().CreateObject(object);
	}

	template <typename Object> void SetObject(jerry_value_t const value, Object object)
	{
		NativeInfo<Object>::Instance().SetObject(value, object);
	}

	template <typename Object> Object GetObject(jerry_value_t const value)
	{
		return NativeInfo<Object>::Instance().GetObject(value);
	}

	template <typename Object> bool IsObject(jerry_value_t const value)
	{
		return NativeInfo<Object>::Instance().IsObject(value);
	}

	// Get data types with template abstraction

	template <typename Type> Type Get(jerry_value_t const value)
	{
		return GetObject<Type>(value);
	}

	template <> std::string Get<std::string>(jerry_value_t const value)
	{
		std::string string(jerry_get_string_size(value), '\0');
		jerry_string_to_char_buffer(value, (jerry_char_t*) string.data(), string.size());
		return string;
	}

	template <> bool Get<bool>(jerry_value_t const value)
	{
		return jerry_get_boolean_value(value);
	}

	template <> double Get<double>(jerry_value_t const value)
	{
		return jerry_get_number_value(value);
	}

	// Create data types with template abstraction

	template <typename Type> jerry_value_t Create(Type object)
	{
		return CreateObject<Type>(object);
	}

	template <> jerry_value_t Create<std::string>(std::string string)
	{
		return jerry_create_string((jerry_char_t*) string.data());
	}

	template <> jerry_value_t Create<bool>(bool boolean)
	{
		return jerry_create_boolean(boolean);
	}

	template <> jerry_value_t Create<double>(double number)
	{
		return jerry_create_number(number);
	}

	// C++ function call wrappers

	template <typename Object, typename... Args, std::size_t... Index>
	jerry_value_t Handler(
		jerry_value_t const value,
		jerry_value_t const obj,
		jerry_value_t const args[],
		jerry_length_t const argc,
		void(Object::*method)(Args...),
		std::index_sequence<Index...>)
	{
		(void) value;
		auto object = Get<Object*>(obj);
		if (object and argc < sizeof...(Args))
		{
			(object->*method)(Get<Args>(args[Index])...);
		}
		return jerry_create_undefined();
	}

	template <typename Object, typename Result, typename... Args, std::size_t... Index>
	jerry_value_t Handler(
		jerry_value_t const value,
		jerry_value_t const obj,
		jerry_value_t const argv[],
		jerry_length_t const argc,
		Result(Object::*method)(Args...),
		std::index_sequence<Index...>)
	{
		auto object = Get<Object*>(obj);
		if (object and argc < sizeof...(Args))
		{
			return Create<Result>((object->*method)(Get<Args>(argv[Index])...));
		}
		return jerry_create_undefined();
	}

	template <typename Object, typename Result, typename... Args, std::size_t... Index>
	jerry_value_t Handler(
		jerry_value_t const value,
		jerry_value_t const obj,
		jerry_value_t const argv[],
		jerry_length_t const argc,
		Result(Object::*method)(Args...))
	{
		return Handler(value, obj, argv, argc, method, std::index_sequence_for<Args...>{});
	}

	template <typename Object, typename Result, typename... Args, std::size_t... Index>
	jerry_value_t Handler(
		jerry_value_t const value,
		jerry_value_t const obj,
		jerry_value_t const argv[],
		jerry_length_t const argc)
	{
		auto method = Get<Result(Object::*)(Args...)>(value);
		return Handler(value, obj, argv, argc, method, std::index_sequence_for<Args...>{});
	}

	template <typename Object, typename Result, typename... Args>
	jerry_value_t NativeInfo<Result(Object::*)(Args...)>::Handler(
		jerry_value_t const value,
		jerry_value_t const obj,
		jerry_value_t const argv[],
		jerry_length_t const argc)
	{
		if (sizeof...(Args) < argc)
		{
			return jerry_create_error(JERRY_ERROR_RANGE, (jerry_char_t*) String(InvalidArgumentRange));
		}
		try
		{
			return ::Handler<Object, Result, Args...>(value, obj, argv, argc);
		}
		catch (std::exception const &exception)
		{
			return jerry_create_error(JERRY_ERROR_COMMON, (jerry_char_t*) exception.what());
		}
	}

	// Some utility classes to automate release of values according to RAII

	struct ScopedValue
	{
		operator jerry_value_t()
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

	struct PropertyDescriptor : jerry_property_descriptor_t
	{
		PropertyDescriptor()
		{
			jerry_init_property_descriptor_fields(this);
		}
		~PropertyDescriptor()
		{
			jerry_free_property_descriptor_fields(this);
		}
		jerry_value_t DefineOwn(jerry_value_t const obj, jerry_value_t const name)
		{
			return jerry_define_own_property(obj, name, this);
		}
		jerry_value_t GetOwn(jerry_value_t const obj, jerry_value_t const name)
		{
			return jerry_get_own_property_descriptor(obj, name, this);
		}
	};

	struct PropertyName : ScopedValue
	{
		PropertyName(char const *name)
		: ScopedValue(jerry_create_string((jerry_char_t*) name))
		{}
	};

	struct Property
	{
		PropertyName name;
		ScopedValue value;
		Property(jerry_value_t const obj, const char *property_name)
		: name(property_name)
		, value(jerry_get_property(obj, name))
		{}
		operator jerry_value_t()
		{
			return value;
		}
	};
}

bool js::ParseAndSaveSnapshot(std::string const &source, enum js::Parse opt, js::Snapshot &buffer)
{
	std::size_t size = buffer.size();
	do
	{
		buffer.resize(size); // hitting this on second run through loop should get correct size
		size = jerry_parse_and_save_snapshot((jerry_char_t*) source.data(), source.size(), opt & Global, opt & Strict, buffer.data(), buffer.size());
		if (0 == size)
		{
			return false;
		}
	}
	while (buffer.size() < size);
	return true;
}

bool js::ExecuteSnapshot(js::Snapshot const &buffer, bool copyBytecode)
{
	ScopedValue const value = jerry_exec_snapshot(buffer.data(), buffer.size(), copyBytecode);
	(void) value; // how to check if error?
	return true;
}


js::Engine::~Engine()
{
	jerry_cleanup();
}
js::Engine::Engine()
{
	jerry_init(JERRY_INIT_EMPTY);
}


