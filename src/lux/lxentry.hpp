#ifndef __lxentry__
#define __lxentry__

#include <cstring>
#include <cstdlib>

struct lux_Entry
{
	const char *key;
	const unsigned value;

	static signed find(const char *str, lux_Entry *list, size_t items)
	{
		lux_Entry key = {str, 0};
		union {
		 lux_Entry *obj;
		 void *addr;
		};
		obj = list;
		addr = bsearch(&key, addr, items, sizeof(lux_Entry), compare);
		return obj ? obj->value : -1;
	}

	static signed mask(const char *str, lux_Entry *list, size_t items)
	{
		static char buf[BUFSIZ];
		const char *del = " ";
		char *tok = strtok(strcpy(buf, str), del);
		int mask = 0;
		while (tok)
		{
		 signed it = find(tok, list, items);
		 if (it < 0)
		  return it;
		 else
		  mask |= it;
		 tok = strtok(NULL, del);	
		}
		return mask;
	}

	static bool check(lux_Entry *list, size_t items)
	{
		bool ordered = true;
		for (int i=0, j=1; j < items; ++i, ++j)
		{
		 int cmp = strcmp(list[i].key, list[j].key);
		 if (!cmp) {
		  fprintf(stderr, "duplicate %s\n", list[i].key);
		  ordered = false;
		 }
		 else
		 if (cmp > 0) {
		  fprintf(stderr, "%s > %s\n", list[i].key, list[j].key);
		  ordered = false;
		 }
		}
		return ordered;
	}

 private:

	static int compare(const void *ptr1, const void *ptr2)
	{
		union {
		 const lux_Entry *e1;
		 const void *p1;
		};
		p1 = ptr1;

		union {
		 const lux_Entry *e2;
		 const void *p2;
		};
		p2 = ptr2;

		return strcmp(e1->key, e2->key);
	}
};

#endif // file
