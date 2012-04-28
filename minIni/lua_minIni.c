
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

	top = lua_gettop(L);
	if ((top == 2) && 
		(lua_isstring(L, -1) == 1) &&
		(lua_isstring(L, -2) == 1))
	{
		section = lua_tostring(L, -2);
		key = lua_tostring(L, -1);
	}
	else
	{
		luaL_error(L, "Wrong Arguments.");
		return 0;
	}

	lua_pushboolean(L, GenerateUpdateFile(input_file_path, output_file_path, product_string, version_string));
	return 1;
}

static const luaL_reg minIni_Functions[]=
{
/*	{"GenUpdateFile",l_GenUpdateFile},*/
	{NULL,NULL}
};

int __declspec(dllexport) luaopen_minIni(lua_State* L)
{
	luaL_openlib(L,"minIni",minIni_Functions,0);
	return 1;
}