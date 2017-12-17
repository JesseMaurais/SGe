#include "JavaScript.hpp"
#include "Stream.hpp"
#include "Error.hpp"
#include "Event.hpp"
#include "std.hpp"
#include <memory>
#include <utility>
#include <functional>
#include <jerryscript-port.h>

namespace
{
	std::string GetString(jerry_value_t const value)
	{
		js::Value const string = jerry_value_to_string(value);
		std::string buffer(jerry_get_string_size(string), '\0');
		jerry_string_to_char_buffer(string, (jerry_char_ptr_t) buffer.data(), buffer.size());
		return buffer;
	}

	// Native object information utility template

	template <typename Object> class NativeInfo : public jerry_object_native_info_t
	{
	public:

		static NativeInfo &Instance()
		{
			static NativeInfo singleton;
			return singleton;
		}

		jerry_value_t CreateObject(Object const &object) const
		{
			jerry_value_t const value = jerry_create_object();
			SetObject(value, object);
			return value;
		}

		jerry_value_t CreateObject(Object const &object, jerry_external_handler_t const handler) const
		{
			jerry_value_t const value = jerry_create_external_function(handler);
			SetObject(value, object);
			return value;
		}

		void SetObject(jerry_value_t const value, Object const &object) const
		{
			Convert cast;
			cast.pointer = new Box(object);
			jerry_set_object_native_pointer(value, cast.address, this);
		}

		Object GetObject(jerry_value_t const value) const
		{
			Convert cast;
			jerry_object_native_info_t const *info;
			if (jerry_get_object_native_pointer(value, &cast.address, &info))
			{
				if (info == this)
				{
					return cast.pointer->object;
				}
			}
			throw std::bad_cast();
		}

		bool IsObject(jerry_value_t const value) const
		{
			jerry_object_native_info_t const *info;
			if (jerry_get_object_native_pointer(value, nullptr, &info))
			{
				if (info == this)
				{
					return true;
				}
			}
			return false;
		}

	private:

		struct Box
		{
			Object object;

			Box(Object &data)
			: object(data)
			{}
		};

		union Convert
		{
			void *address;
			Box *pointer;
		};

		static void Free(void *native)
		{
			if (native) // not nullptr
			{
				Convert cast;
				cast.address = native;
				delete cast.pointer;
			}
		}

		NativeInfo()
		{
			free_cb = Free;
		}
	};

	// Simplified syntax for the native object singleton

	template <typename Object> jerry_value_t CreateObject(Object const &object)
	{
		return NativeInfo<Object>::Instance().CreateObject(object);
	}

	template <typename Object> jerry_value_t CreateObject(Object const &object, jerry_external_handler_t const handler)
	{
		return NativeInfo<Object>::Instance().CreateObject(object, handler);
	}

	template <typename Object> void SetObject(jerry_value_t const value, Object const &object)
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

	// Create data types with template abstraction

	template <typename Type> jerry_value_t Create(Type const &data)
	{
		return CreateObject<Type>(data);
	}

	template <> jerry_value_t Create<std::string>(std::string const &string)
	{
		return jerry_create_string((jerry_char_ptr_t) string.data());
	}

	template <> jerry_value_t Create<bool>(bool const &boolean)
	{
		return jerry_create_boolean(boolean);
	}

	template <> jerry_value_t Create<double>(double const &number)
	{
		return jerry_create_number(number);
	}

	// Get data types with template abstraction

	template <typename Type> Type Get(jerry_value_t const value)
	{
		return GetObject<Type>(value);
	}

	template <> std::string Get<std::string>(jerry_value_t const value)
	{
		return GetString(value);
	}

	template <> bool Get<bool>(jerry_value_t const value)
	{
		return jerry_get_boolean_value(value);
	}

	template <> double Get<double>(jerry_value_t const value)
	{
		return jerry_get_number_value(value);
	}

	// As data types with template abstraction

	template <typename Type> Type As(jerry_value_t const value)
	{
		return GetObject<Type>(value);
	}

	template <> std::string As<std::string>(jerry_value_t const value)
	{
		js::Value converted = jerry_value_to_string(value);
		return Get<std::string>(converted);
	}

	template <> bool As<bool>(jerry_value_t const value)
	{
		js::Value converted = jerry_value_to_string(value);
		return Get<bool>(converted);
	}

	template <> double As<double>(jerry_value_t const value)
	{
		js::Value converted = jerry_value_to_number(value);
		return Get<double>(converted);
	}

	// C++ function wrapper

	template <typename Signature, typename Object, typename... Args, std::size_t... Index>
	jerry_value_t Handler
	(
		jerry_value_t const value, jerry_value_t const that,
		jerry_value_t const argv[], jerry_length_t const argc,
		std::index_sequence<Index...>
	)
	{
		using Function = std::function<Signature>;
		using Pointer = std::shared_ptr<Object>;

		if (sizeof...(Args) == argc)
		{
			if constexpr (not std::is_void<Object>::value)
			{
				auto function = Get<Function>(value);
				auto pointer = Get<Pointer>(that);

				if constexpr (not std::is_void<typename Function::result_type>::value)
				{
					return Create(function(pointer.get(), As<Args>(argv[Index])...));
				}
				else
				{
					Create(function(pointer.get(), As<Args>(argv[Index])...));
				}
			}
			else
			{
				auto function = Get<Function>(value);

				if constexpr (not std::is_void<typename Function::result_type>::value)
				{
					return Create(function(As<Args>(argv[Index])...));
				}
				else
				{
					Create(function(As<Args>(argv[Index])...));
				}
			}
		}
		return jerry_create_undefined();
	}

	template <typename Signature, typename Object, typename... Args>
	jerry_value_t Handler
	(
		jerry_value_t const value, jerry_value_t const that,
		jerry_value_t const argv[], jerry_length_t const argc
	)
	{
		try
		{
			auto const argn = std::index_sequence_for<Args...>();
			return Handler<Signature, Object, Args...>(value, that, argv, argc, argn);
		}
		catch (std::exception const &exception)
		{
			auto const message = (jerry_char_ptr_t) exception.what();
			jerry_value_t value = jerry_create_string(message);
			jerry_value_set_error_flag(&value);
			return value;
		}
	}

	template <typename Signature, typename Object, typename... Args>
	jerry_value_t CreateHandler(std::function<Signature> function)
	{
		jerry_value_t const value = CreateObject(function, Handler<Signature, Object, Args...>);
		assert(jerry_value_is_function(value));
		return value;
	}

	template <typename Object>
	jerry_value_t CreateShared(std::shared_ptr<Object> shared)
	{
		jerry_value_t const value = CreateObject(shared);
		assert(jerry_value_is_object(value));
		return value;
	}

	template <typename Signature>
	jerry_value_t CreateConstructor(Signature &&constructor)
	{
		jerry_value_t const value = CreateHandler(constructor);
		assert(jerry_value_is_constructor(value));
		return value;
	}

	// More utility functions for setting properties

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

		operator jerry_value_t() const
		{
			return value;
		}
	};

	struct Property
	{
		js::Value name;
		js::Value value;

		Property(char const *property_name, jerry_value_t const property_value)
		: name(jerry_create_string((jerry_char_ptr_t) property_name))
		, value(property_value)
		{}

		operator jerry_value_t() const
		{
			return value;
		}
	};

	bool SetProperty(jerry_value_t const obj, Property const &prop)
	{
		js::Value const value = jerry_set_property(obj, prop.name, prop.value);
		return js::CheckError(value);
	}

	void Evaluate(SDL_Event const &event)
	{
		bool const strict = false != event.user.code;
		auto const begin = (jerry_char_ptr_t) event.user.data1;
		auto const end = (jerry_char_ptr_t) event.user.data2;

		struct ScopedSignal
		{
			~ScopedSignal()
			{
				SignalStream(); // notify thread to continue
			}

		} scopedSignal;

		// Parse the input string into an executable function
		js::Value const function = jerry_parse(begin, end-begin, strict);
		if (js::CheckError(function))
		{
			SDL::LogError("jerry_parse");
			return;
		}

		// Execute the function argument
		js::Value const result = jerry_run(function);
		if (js::CheckError(result))
		{
			SDL::LogError("jerry_run");
			return;
		}

		// Print the result value as string and flush
		std::cout << GetString(result) << std::endl;
	}
}

bool js::Init(jerry_init_flag_t const flags)
{
	jerry_init(flags);
	if (std::atexit(jerry_cleanup))
	{
		SDL::SetError(CannotMakeExit, "jerry_cleanup");
		return false;
	}

	static ScopedEventHandler
		handler(SDL::UserEvent(EvaluateScript), Evaluate);

	return true;
}

bool js::SetError(jerry_value_t value)
{
	if (jerry_is_feature_enabled(JERRY_FEATURE_ERROR_MESSAGES))
	{
		jerry_value_clear_error_flag(&value);
		return SDL::SetError(GetString(value));
	}
	else
	{
		return SDL::SetError(NoDebugger);
	}
}

bool js::CheckError(jerry_value_t value)
{
	return jerry_value_has_error_flag(value) and js::SetError(value);
}


// Implementation of the JerryScript port API

void jerry_port_fatal(jerry_fatal_code_t code)
{
	SDL::Log("jerry(%1) fatal", code);
	std::exit(0);
}

void jerry_port_log(jerry_log_level_t level, const char *format, ...)
{
	va_list args;
	va_start (args, format);
	char string[BUFSIZ];
	std::vsnprintf(string, sizeof(string), format, args);
	va_end (args);
	SDL::Log("jerry(%1) %2", level, string);
}

bool jerry_port_get_time_zone(jerry_time_zone_t *tz)
{
	(void)tz;
	return false;
}

double jerry_port_get_current_time()
{
	return static_cast<double>(std::time(nullptr));
}
