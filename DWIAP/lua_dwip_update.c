


#include <stdlib.h>
#include <string.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "dwip_update.h"

static int l_GenUpdateFile(lua_State* L)
{
	const char* input_file_path = NULL;
	const char* output_file_path = NULL;
	const char* product_string = NULL;
	const char* version_string = NULL;

	input_file_path = luaL_checkstring(L, 1);
	output_file_path = luaL_checkstring(L, 2);
	product_string = luaL_checkstring(L, 3);
	version_string = luaL_checkstring(L, 4);

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