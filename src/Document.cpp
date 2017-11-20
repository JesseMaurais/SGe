#include "Document.hpp"
#include "JavaScript.hpp"


void Document::Start(char const *name, char const **attributes)
{

}

void Document::End(char const *name)
{

}

/*

-bool Document::Load(lua_State *state, int schema, SDL_RWops *ops)
-{
-       // New result
-       lua_newtable(state);
-       root = lua_gettop(state);
-       // Use global if no table given
-       if (lua_isnoneornil(state, schema))
-       {
-               lua_pushglobaltable(state);
-               schema = lua_gettop(state);
-       }
-       // Prepare
-       index.push(0);
-       this->state = state;
-       this->schema = schema;
-       // Start parsing input
-       bool okay = XML::Load(ops);
-       // Save only the result
-       lua_settop(state, root);
-       index.pop();
-       return okay;
-}

-bool Document::Load(lua_State *state, int schema, const char *path)
-{
-       // New result
-       lua_newtable(state);
-       root = lua_gettop(state);
-       // Use global if no table given
-       if (lua_isnoneornil(state, schema))
-       {
-               lua_pushglobaltable(state);
-               schema = lua_gettop(state);
-       }
-       // Prepare
-       index.push(0);
-       this->state = state;
-       this->schema = schema;
-       // Start parsing input
-       bool okay = XML::Load(path);
-       // Save only the result
-       lua_settop(state, root);
-       index.pop();
-       return okay;
-}

-void Document::Start(const char *name, const char **attributes)
-{
-       // Node index in the parent
-       lua_pushinteger(state, ++index.top());
-       lua_newtable(state);
-       // Table index in the stack
-       int table = lua_gettop(state);
-       // Using named meta-table from the schema
-       if (luaL_getsubtable(state, schema, name))
-       {
-               SDL_Log(String(CannotFindSchema), name);
-       }
-       lua_setmetatable(state, table);
-       // Set all key/value pairs as fields in this table
-       for (int key = 0, value = 1; attributes[key]; key += 2, value += 2)
-       {
-               // Value 'id' is a unique name for this node
-               if (not SDL_strcasecmp(attributes[key], "id"))
-               {
-                       // Check that name is not already being used
-                       if (lua_getfield(state, root, attributes[value]))
-                       {
-                               SDL_Log(String(NameNotUnique), attributes[value]);
-                       }
-                       lua_pop(state, 1);
-                       // Make root.value = table
-                       lua_pushvalue(state, table);
-                       lua_setfield(state, root, attributes[value]);
-               }
-               // Make table.key = value
-               lua_pushstring(state, attributes[value]);
-               lua_setfield(state, table, attributes[key]);
-       }
-       index.push(0);
-}

-void Document::End(const char *name)
-{
-       (void) name;
-       // Position under stack
-       constexpr int table = -3;
-       // Make parent[index] = table
-       lua_settable(state, table);
-       index.pop(); // unwind
-}


 */
