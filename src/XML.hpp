#ifndef XML_hpp
#define XML_hpp

struct SDL_RWops;
struct XML_ParserStruct;

struct XML
{
	XML(char const *encoding = nullptr);
	virtual ~XML();

	bool Load(SDL_RWops *ops);
	bool Load(char const *path);

  protected:

	virtual void Start(char const *name, char const **attributes) = 0;
	virtual void End(char const *name) = 0;

  private:

	XML_ParserStruct *parser;
};

#endif // file
