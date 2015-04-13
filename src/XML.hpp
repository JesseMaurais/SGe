#ifndef __XML__
#define __XML__

#include <string>
#include <expat.h>
#include <SDL2/SDL_rwops.h>

struct XML
{
	XML_Parser parser;
	XML(const char *encoding=NULL);
	~XML();

	bool Load(SDL_RWops *ops);
	bool Load(const char *path);

  protected:

	virtual void Start(const char *name, const char **attributes);
	virtual void End(const char *name);
	virtual void CData(const char *string, int length);

  private:

	static void start(void *user, const char *name, const char **attr);
	static void end(void *user, const char *name);
	static void cdata(void *user, const char *string, int length);
};

#endif // file

