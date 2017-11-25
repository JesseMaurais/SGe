#include "Document.hpp"
#include "JavaScript.hpp"
#include "XML.hpp"
#include <vector>
#include <stack>

namespace
{
	class DOM : public XML
	{
	public:

		DOM(jerry_value_t const value)
		: schema(value)
		{}

		bool Load(jerry_value_t obj, SDL_RWops *ops)
		{
			stack.push(obj);
			std::vector<jerry_value_t> empty;
			nodes.push(empty);
			return XML::Load(ops);
		}

	protected:

		void Start(char const *element, char const **attributes) override
		{
			js::Value type = element;
			js::Value function = jerry_get_property(schema, type);
			if (jerry_value_is_constructor(function))
			{
				js::Value args = jerry_create_object();
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

				auto obj = jerry_construct_object(function, &args.value, 1);
				if (js::CheckError(obj))
				{
					// handle error
				}

				nodes.top().push_back(obj);
				std::vector<jerry_value_t> empty;
				nodes.push(empty);
				stack.push(obj);
			}
			else
			{
				// handle error
			}
		}

		void End(char const *element) override
		{
			auto obj = stack.top();
			auto size = nodes.top().size();

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

			js::Value name = "child";
			js::Value error = jerry_set_property(obj, name, array);
			if (js::CheckError(error))
			{
				// handle error
			}

			nodes.pop();
			stack.pop();
		}

	private:

		std::stack<std::vector<jerry_value_t>> nodes;
		std::stack<jerry_value_t> stack;
		jerry_value_t const schema;
	};
}

Document::Document()
: self(jerry_create_object())
{}

Document::~Document()
{
	jerry_release_value(self);
}

bool Document::Load(SDL_RWops *ops)
{
	js::Value global = jerry_get_global_object();
	return DOM(global).Load(self, ops);
}
