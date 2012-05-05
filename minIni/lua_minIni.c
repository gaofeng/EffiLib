
#include <stdlib.h>
#include <string.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "minIni.h"

#define FN_KEY "filename"

static int l_ini_open(lua_State* L)
{
    const char* filename = NULL;
    int top;

    top = lua_gettop(L);

    if (top != 1)
    {
        luaL_error(L, "Must have only one argument!");
        return 0;
    }

    filename = luaL_checkstring(L, 1);
    lua_setfield(L, LUA_ENVIRONINDEX, FN_KEY);
    lua_pushboolean(L, 1);
    return 1;
}

static int l_ini_gets(lua_State* L)
{
	int top;
	const char* section = NULL;
	const char* key = NULL;
    const char* default_value = "";
    const char* filename = NULL;
    char* buffer = NULL;
    int value_len = 0;

    section = luaL_checkstring(L, 1);
    key = luaL_checkstring(L, 2);

	top = lua_gettop(L);
    if (top == 3)
    {
        default_value = luaL_checkstring(L, 3);
    }

    lua_getfield(L, LUA_ENVIRONINDEX, FN_KEY);
    if (lua_isnil(L, -1))
    {
        luaL_error(L, "Must call open first!");
        return 0;
    }
    filename = lua_tostring(L, -1);

    buffer = (char*)malloc(INI_BUFFERSIZE + 1);
    if (buffer == NULL)
    {
        luaL_error(L, "Not enough memory.");
        return 0;
    }
    memset(buffer, 0x00, INI_BUFFERSIZE + 1);
    value_len = ini_gets(section, key, default_value, buffer, INI_BUFFERSIZE, filename);

	lua_pushlstring(L, buffer, value_len);
	return 1;
}

static const luaL_reg minIni_Functions[]=
{
    {"open",l_ini_open},
	{"gets",l_ini_gets},
	{NULL,NULL}
};

int __declspec(dllexport) luaopen_minIni(lua_State* L)
{
    lua_newtable(L);
    lua_replace(L, LUA_ENVIRONINDEX);
	luaL_openlib(L,"minIni",minIni_Functions,0);
	return 1;
}