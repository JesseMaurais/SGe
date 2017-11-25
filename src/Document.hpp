#ifndef Document_hpp
#define Document_hpp

#include "JavaScript.hpp"
#include "XML.hpp"
#include <string>
#include <stack>
#include <vector>

class DOM : public XML
{
public:

	DOM(js::Value const &schema, char const *encoding = nullptr);
	~DOM();

	js::Value Parse(SDL_RWops *ops);
	js::Value ParseFromFile(std::string const &path);
	js::Value ParseFromString(std::string const &text);

protected:

	void Start(char const *element, char const **attributes) override;
	void End(char const *element) override;

private:

	std::stack<std::vector<js::Value>> nodes;
	std::stack<js::Value> stack;
	js::Value const schema;
};

#endif // file
