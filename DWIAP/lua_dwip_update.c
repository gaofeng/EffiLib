


#include <stdlib.h>
#include <string.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "dwip_update.h"

static int l_GenUpdateFile(lua_State* L)
{
	int top;
	const char* input_file_path = NULL;
	const char* output_file_path = NULL;
	const char* product_string = NULL;
	const char* version_string = NULL;

	top = lua_gettop(L);
	if ((top == 4) && 
		(lua_isstring(L, -1) == 1) && 
		(lua_isstring(L, -2) == 1) &&
		(lua_isstring(L, -3) == 1) &&
		(lua_isstring(L, -4) == 1))
	{
		input_file_path = lua_tostring(L, -4);
		output_file_path = lua_tostring(L, -3);
		product_string = lua_tostring(L, -2);
		version_string = lua_tostring(L, -1);
	}
	else
	{
		luaL_error(L, "Wrong Arguments.");
		return 0;
	}

	lua_pushboolean(L, GenerateUpdateFile(input_file_path, output_file_path, product_string, version_string));
	return 1;
}

static const luaL_reg DWIP_Update_Functions[]=
{
	{"GenUpdateFile",l_GenUpdateFile},
	{NULL,NULL}
};

int __declspec(dllexport) luaopen_dwiap(lua_State* L)
{
	luaL_openlib(L,"dwiap",DWIP_Update_Functions,0);
	return 1;
}