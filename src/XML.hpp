#ifndef XML_hpp
#define XML_hpp

struct SDL_RWops;
struct XML_ParserStruct;

struct XML
{
	XML(const char *encoding=nullptr);
	virtual ~XML();

	bool Load(SDL_RWops *ops);
	bool Load(const char *path);

  protected:

	virtual void Start(const char *name, const char **attributes) = 0;
	virtual void End(const char *name) = 0;

  private:

	XML_ParserStruct *parser;
};

#endif // file
