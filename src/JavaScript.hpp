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
	bool SetError(jerry_value_t value);
	/// Set the SDL error string to the current error code. Returns true when an error exists
	bool CheckError(jerry_value_t value);

	/// Utility class to automate release of values according to RAII
	struct Value
	{
		operator jerry_value_t() const
		{
			return value;
		}

		Value(jerry_value_t const release)
		{
			value = release;
		}

		Value(char const *string)
		: Value(jerry_create_string((jerry_char_ptr_t) string))
		{}

		Value(double number)
		: Value(jerry_create_number(number))
		{}

		Value(bool boolean)
		: Value(jerry_create_boolean(boolean))
		{}

		Value(js::Value const &acquire)
		: Value(jerry_acquire_value(acquire))
		{}

		~Value()
		{
			jerry_release_value(value);
		}

		jerry_value_t value;
	};

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
		Value name;
		Value value;

		Property(char const *property_name, jerry_value_t const property_value)
		: name(jerry_create_string((jerry_char_ptr_t) property_name))
		, value(property_value)
		{}

		operator jerry_value_t() const
		{
			return value;
		}
	};
}

#endif // file
