#include "GUI.hpp"
#include "SDL.hpp"
#include <expat.h>
#include <cassert>
#include <cstring>
#include <stack>
#include <queue>
#include <map>

typedef GUI *(*Construct)(const char **attributes);

struct Compare
{
	bool operator()(const char *s, const char *s2)
	{
		return SDL_strcasecmp(s, s2) < 0;
	}
};

static std::map<const char*, Construct, Compare> map;

static std::stack<GUI*> stack;

static XML_Parser parser;

static void Start(void *node, const char *name, const char **attributes)
{
	union {
	 void *data;
	 GUI *obj;
	};
	data = node;
	stack.push(obj);
	obj = map.at(name)(attributes);
	XML_SetUserData(parser, data);
}

static void End(void *node, const char *name)
{
	union {
	 void *data;
	 GUI *obj;
	};
	data = node;
	obj->Done();
	GUI *top = stack.top();
	XML_SetUserData(parser, top);
	top->AddChild(obj);
	stack.pop();
}

void Register(const char *name, Construct construct)
{
	if (!construct)
	{
		if (!name) map.clear();
		else map.erase(name);
	}
	else
	if (name) map[name] = construct;
}

GUI::GUI()
{
	x = y = w = h = 0;
	next = childs = NULL;
}

bool GUI::Load(SDL_RWops *ops)
{
	new (this) GUI;
	assert(ops);
	parser = XML_ParserCreate(NULL);
	if (!parser) return false;
	XML_SetUserData(parser, this);
	XML_SetElementHandler(parser, Start, End);
	size_t sz;
	void *buf;
	do
	{
		buf = XML_GetBuffer(parser, BUFSIZ);
		sz = SDL_RWread(ops, buf, sizeof(char), BUFSIZ);
		if (sz < 0) SDL_perror("SDL_RWread");
		else
		if (!XML_ParseBuffer(parser, sz, !sz))
		{
			XML_Error code = XML_GetErrorCode(parser);
			const char *string = XML_ErrorString(code);
			int line = XML_GetCurrentLineNumber(parser);
			int col = XML_GetCurrentColumnNumber(parser);
			SDL_Log("XML (%d, %d) %s", line, col, string);
			break;
		}
	}
	while (sz > 0);
	XML_ParserFree(parser);
	Done();
	x = y = SDL_WINDOWPOS_UNDEFINED;
	return true;
}

bool GUI::Load(const char *path)
{
	bool result = false;
	SDL_RWops *ops = SDL_RWFromFile(path, "r");
	if (!ops) SDL_perror("SDL_RWFromFile");
	else
	{
	 result = Load(ops);
	 SDL_RWclose(ops);
	}
	return result; 
}

bool GUI::Load(FILE *file)
{
	bool result = false;
	SDL_RWops *ops = SDL_RWFromFP(file, SDL_FALSE);
	if (!ops) SDL_perror("SDL_RWFromFP");
	else
	{
	 result = Load(ops);
	 SDL_RWclose(ops);
	}
	return result;
}

GUI::~GUI(void)
{
	GUI *obj = childs;
	while (obj)
	{
	 GUI *next = obj->next;
	 delete obj;
	 obj = next;
	}
}

void GUI::AddChild(GUI *obj)
{
	GUI *end = childs;
	if (!end) childs = obj;
	else
	{
	 while (end->next) end = end->next;
	 end->next = obj;
	}
}

void GUI::Draw()
{
	ChildDraw();
}

void GUI::Done()
{
	ChildRestack();
	ChildUnionRect();
}

void GUI::ChildUnionRect()
{
	GUI *obj = childs;
	if (obj)
	{
	 x = obj->x;
	 y = obj->y;
	 w = obj->w;
	 h = obj->h; 
	 obj = obj->next;
	 while (obj)
	 {
	  SDL_UnionRect(this, obj, this);
	  obj = obj->next;
	 }
	}
}

void GUI::ChildRestack()
{
	GUI *obj = childs;
	SDL_Point point = {x, y};
	while (obj)
	{
	 obj->x = point.x;
	 obj->y = point.y;
	 point.y += obj->h;
	 obj = obj->next;
	}
}

void GUI::ChildDraw()
{
	GUI *obj = childs;
	while (obj)
	{
	 obj->Draw();
	 obj->ChildDraw();
	 obj = obj->next;
	}
}

