
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
    int string_len = 0;

	if (lua_isnil(L, 1) != 0)
	{
		section = NULL;
	}
	else
	{
		section = luaL_checkstring(L, 1);
	}
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
    string_len = ini_gets(section, key, default_value, buffer, INI_BUFFERSIZE, filename);

	lua_pushlstring(L, buffer, string_len);
	free(buffer);
	return 1;
}

static int l_ini_getl(lua_State* L)
{
	int top;
	const char* section = NULL;
	const char* key = NULL;
	long default_value = 0;
	const char* filename = NULL;
	long result_num = 0;

	if (lua_isnil(L, 1) != 0)
	{
		section = NULL;
	}
	else
	{
		section = luaL_checkstring(L, 1);
	}
	key = luaL_checkstring(L, 2);

	top = lua_gettop(L);
	if (top == 3)
	{
		default_value = (long)luaL_checknumber(L, 3);
	}

	lua_getfield(L, LUA_ENVIRONINDEX, FN_KEY);
	if (lua_isnil(L, -1))
	{
		luaL_error(L, "Must call open first!");
		return 0;
	}
	filename = lua_tostring(L, -1);

	result_num = ini_getl(section, key, default_value, filename);

	lua_pushnumber(L, (lua_Number)result_num);
	return 1;
}

static int l_ini_puts(lua_State* L)
{
	const char* section = NULL;
	const char* key = NULL;
	const char* value = NULL;
	const char* filename = NULL;
	int result = 0;

	if (lua_isnil(L, 1) != 0)
	{
		section = NULL;
	}
	else
	{
		section = luaL_checkstring(L, 1);
	}
	key = luaL_checkstring(L, 2);

	value = luaL_checkstring(L, 3);

	lua_getfield(L, LUA_ENVIRONINDEX, FN_KEY);
	if (lua_isnil(L, -1))
	{
		luaL_error(L, "Must call open first!");
		return 0;
	}
	filename = lua_tostring(L, -1);

	result = ini_puts(section, key, value, filename);

	lua_pushboolean(L, result);
	return 1;
}

static const luaL_reg minIni_Functions[]=
{
    {"open",l_ini_open},
	{"gets",l_ini_gets},
	{"getl",l_ini_getl},
	{"puts",l_ini_puts},
	{NULL,NULL}
};

int __declspec(dllexport) luaopen_minIni(lua_State* L)
{
    lua_newtable(L);
    lua_replace(L, LUA_ENVIRONINDEX);
	luaL_openlib(L,"minIni",minIni_Functions,0);
	return 1;
}