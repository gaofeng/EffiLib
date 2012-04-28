
#include <stdlib.h>
#include <string.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "minIni.h"

static int l_ini_gets(lua_State* L)
{
	int top;
	const char* section = NULL;
	const char* key = NULL;
    const char* default_value = NULL;
    const char* filename = NULL;
    char* buffer = NULL;
    int value_len = 0;

	top = lua_gettop(L);
	if (((top == 3) || (top == 4)) && 
		(lua_isstring(L, -1) == 1) &&
		(lua_isstring(L, -2) == 1))
	{
		section = lua_tostring(L, 1);
		key = lua_tostring(L, 2);
        if (top == 4)
        {
            default_value = lua_tostring(L, 3);
        }
        filename = lua_tostring(L, -1);
	}
	else
	{
		luaL_error(L, "Wrong Arguments.");
		return 0;
	}

    buffer = (char*)malloc(INI_BUFFERSIZE + 1);
    if (buffer == NULL)
    {
        luaL_error(L, "Not enough memory.");
        return 0;
    }
    memset(buffer, 0x00, INI_BUFFERSIZE + 1);
    value_len = ini_gets(section, key, default_value, buffer, INI_BUFFERSIZE, filename);
    lua_pushstring(L, buffer);
	return 1;
}

static const luaL_reg minIni_Functions[]=
{
	{"gets",l_ini_gets},
	{NULL,NULL}
};

int __declspec(dllexport) luaopen_minIni(lua_State* L)
{
	luaL_openlib(L,"minIni",minIni_Functions,0);
	return 1;
}