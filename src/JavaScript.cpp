#include "JavaScript.hpp"
#include "Stream.hpp"
#include "Error.hpp"
#include "Event.hpp"
#include "std.hpp"
#include <ctime>
#include <memory>
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

		static NativeInfo &Instance() const
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

		typedef struct Box
		{
			Object object;

			Box(Object &data)
			: object(data)
			{}

		} *Pointer;

		union Convert
		{
			void *address;
			Pointer pointer;
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

	template <> jerry_value_t Create<double>(double &number)
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

	// C++ function call wrappers first level (4 base cases)

	template <typename... Args, std::size_t... Index>
	jerry_value_t Handler
	(
		void(*method)(Args...),
		jerry_value_t const args[], jerry_length_t const argc,
		std::index_sequence<Index...>
	)
	{
		method(As<Args>(args[Index])...);
		return jerry_create_undefined();
	}

	template <typename Result, typename... Args, std::size_t... Index>
	jerry_value_t Handler
	(
		Result(*method)(Args...),
		jerry_value_t const argv[], jerry_length_t const argc,
		std::index_sequence<Index...>
	)
	{
		return Create<Result>(method(As<Args>(argv[Index])...));
	}

	template <typename Object, typename... Args, std::size_t... Index>
	jerry_value_t Handler
	(
		void(Object::*method)(Args...), Object *object,
		jerry_value_t const args[], jerry_length_t const argc,
		std::index_sequence<Index...>
	)
	{
		(object->*method)(As<Args>(args[Index])...);
		return jerry_create_undefined();
	}

	template <typename Object, typename Result, typename... Args, std::size_t... Index>
	jerry_value_t Handler
	(
		Result(Object::*method)(Args...), Object *object,
		jerry_value_t const argv[], jerry_length_t const argc,
		std::index_sequence<Index...>
	)
	{
		return Create<Result>((object->*method)(As<Args>(argv[Index])...));
	}

	// C++ function wrappers second level (2 base cases)

	template <typename Result, typename... Args>
	jerry_value_t Handler
	(
		Result(*method)(Args...),
		jerry_value_t const argv[], jerry_length_t const argc
	)
	{
		return Handler(method, argv, argc, std::index_sequence_for<Args...>{});
	}

	template <typename Object, typename Result, typename... Args>
	jerry_value_t Handler
	(
		Result(Object::*method)(Args...), Object *object,
		jerry_value_t const argv[], jerry_length_t const argc
	)
	{
		return Handler(method, object, argv, argc, std::index_sequence_for<Args...>{});
	}

	// C++ function wrappers third level (get object and method)

	template <typename Result, typename... Args>
	jerry_value_t Handler
	(
		jerry_value_t const value,
		jerry_value_t const argv[], jerry_length_t const argc
	)
	{
		auto method = Get<Result(*)(Args...)>(value);
		if (method)
		{
			return Handler(method, argv, argc, std::index_sequence_for<Args...>{});
		}
		return jerry_create_undefined();
	}

	template <typename Object, typename Result, typename... Args>
	jerry_value_t Handler
	(
		jerry_value_t const value, jerry_value_t const obj,
		jerry_value_t const argv[], jerry_length_t const argc
	)
	{
		auto method = Get<Result(Object::*)(Args...)>(value);
		auto object = Get<Object*>(obj);
		if (method and object)
		{
			return Handler(method, object, argv, argc, std::index_sequence_for<Args...>{});
		}
		return jerry_create_undefined();
	}

	// C++ function wrappers fourth level (native info)

	template <typename Result, typename... Args>
	jerry_value_t NativeInfo<Result(*)(Args...)>::Handler
	(
		jerry_value_t const value, jerry_value_t const obj,
		jerry_value_t const argv[],	jerry_length_t const argc
	)
	{
		if (sizeof...(Args) < argc)
		{
			auto error = (jerry_char_ptr_t) String(InvalidArgumentRange);
			return jerry_create_error(JERRY_ERROR_RANGE, error);
		}
		try // report exceptions to jerry
		{
			(void) obj; // unused
			return ::Handler<Result, Args...>(value, argv, argc);
		}
		catch (std::exception const &exception)
		{
			return jerry_create_error(JERRY_ERROR_COMMON, (jerry_char_ptr_t) exception.what());
		}
	}

	template <typename Object, typename Result, typename... Args>
	jerry_value_t NativeInfo<Result(Object::*)(Args...)>::Handler
	(
		jerry_value_t const value, jerry_value_t const obj,
		jerry_value_t const argv[], jerry_length_t const argc
	)
	{
		if (sizeof...(Args) < argc)
		{
			auto error = (jerry_char_ptr_t) String(InvalidArgumentRange);
			return jerry_create_error(JERRY_ERROR_RANGE, error);
		}
		try // report exceptions to jerry
		{
			return ::Handler<Object, Result, Args...>(value, obj, argv, argc);
		}
		catch (std::exception const &exception)
		{
			return jerry_create_error(JERRY_ERROR_COMMON, (jerry_char_ptr_t) exception.what());
		}
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

	template <typename Signature>
	jerry_value_t Constructor(Signature &&constructor)
	{
		jerry_value_t const value = Create(constructor);
		assert(jerry_value_is_constructor(value));
		return value;
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


namespace
{
	class SomeClass
	{
	public:
		std::string Message(double n)
		{
			std::string message;
			for (int i = 0; i < n; ++i)
				message += "Hello! ";
			return message;
		}
	};

	template <> void NativeInfo<SomeClass*>::Prototype(jerry_value_t const proto)
	{
		SetProperty(proto, Property("Message", Create(&SomeClass::Message)));
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
	SDL::Log("Fatal: %1", code);
	std::exit(0);
}

void jerry_port_log(jerry_log_level_t level, const char *format, ...)
{
	SDL::Log("Log: %1", level);

	va_list args;
	va_start (args, format);
	char string[BUFSIZ];
	std::vsnprintf(string, sizeof(string), format, args);
	va_end (args);
	SDL_Log("%s", string);
}

bool jerry_port_get_time_zone(jerry_time_zone_t *tz)
{
	SDL::Log("TZ data");

	tzset();
	tz->offset = timezone;
	tz->daylight_saving_time = daylight;
	return true;
}

double jerry_port_get_current_time()
{
	SDL::Log("Time");

	return static_cast<double>(std::time(nullptr));
}
