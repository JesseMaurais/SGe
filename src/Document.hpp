#ifndef Document_hpp
#define Document_hpp

#include "XML.hpp"

class Document : public XML
{
public:

	bool Load(unsigned schema, SDL_RWops *ops);
	bool Load(unsigned schema, char const *path);

protected:

	void Start(char const *name, char const **attributes) override;
	void End(char const *name) override;
};

#endif // file
