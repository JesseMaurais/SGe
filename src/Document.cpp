#include "Document.hpp"


DOM::DOM(js::Value const &value, char const *encoding)
: XML(encoding)
, schema(value)
{}

js::Value DOM::Parse(SDL_RWops *ops)
{
	js::Value root = jerry_create_object();
	std::vector<js::Value> empty;
	stack.push(root);
	nodes.push(empty);
	try
	{
		if (XML::Parse(ops))
		{
			return root;
		}
	}
	catch (std::exception const &exception)
	{
		SDL::SetError(CaughtException, exception.what());
	}
	return jerry_create_undefined();
}


void DOM::Start(char const *element, char const **attributes)
{
	// Create an attribute list for the node

	js::Value args = jerry_create_object();
	if (js::CheckError(args))
	{
		// handle error
	}
	for (auto i=0, j=1; attributes[j]; ++i, ++j)
	{
		js::Value name = attributes[i];
		js::Value value = attributes[j];
		js::Value err = jerry_set_property(args, name, value);
		if (js::CheckError(err))
		{
			// handle error
		}
	}

	// Construct the node from schema's element reference

	js::Value type = element;
	js::Value function = jerry_get_property(schema, type);
	if (not jerry_value_is_constructor(function))
	{
		// handle error
	}

	auto obj = jerry_construct_object(function, &args.value, 1);
	if (js::CheckError(obj))
	{
		// handle error
	}

	// Adjust the stack for the new element

	nodes.top().push_back(obj);
	std::vector<js::Value> empty;
	nodes.push(empty);
	stack.push(obj);
}

void DOM::End(char const *element)
{
	auto obj = stack.top();
	auto size = nodes.top().size();

	// Put the child nodes into an array

	js::Value array = jerry_create_array(size);
	for (auto index = 0; index < size; ++index)
	{
		js::Value node = nodes.top().at(index);
		js::Value err = jerry_set_property_by_index(array, index, node);
		if (js::CheckError(err))
		{
			// handle error
		}
	}

	// Add this array nodes to the parent

	js::Value name = "childNodes";
	js::Value error = jerry_set_property(obj, name, array);
	if (js::CheckError(error))
	{
		// handle error
	}

	// Unwind the stack

	nodes.pop();
	stack.pop();
}

