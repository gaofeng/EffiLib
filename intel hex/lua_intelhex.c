
#include <stdlib.h>
#include <string.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "intel.h"

static int l_HexToBin(lua_State* L)
{
	int top = 0;
	const char* hex_file = NULL;
	char* bin_file = NULL;
	int str_len;
	IntelHexFormat* ihf = NULL;
	bool result = FALSE; 

	top = lua_gettop(L);
	if ((top == 1) && (lua_isstring(L, -1) == 1))
	{
		hex_file = lua_tostring(L,-1);
		str_len = strlen(hex_file);
		if (strcmp(hex_file + str_len - 4, ".hex") == 0)
		{
			bin_file = malloc(str_len + 1);
			if (bin_file == NULL)
			{
				printf("Error: Not enough memory.\n");
				return 0;
			}
			memset(bin_file, 0x00, str_len + 1);
			strcpy(bin_file, hex_file);
			memcpy(bin_file + str_len - 4, ".bin", 4);
		}
	}
	else if ((top == 2) && (lua_isstring(L, -1) == 1) && (lua_isstring(L, -2) == 1))
	{
		hex_file = lua_tostring(L, -2);
		bin_file = (char*)lua_tostring(L, -1);
	}
	else
	{
		luaL_error(L, "Wrong Arguments.");
		return 0;
	}

	/*读入HEX文件*/
	ihf = IntelHexFileInput(hex_file);
	if (ihf)
	{
		/*数据间隙用0xFF填充*/
		IntelHexFileFillEmptyValue(ihf, 0xFF);
		/*输出到BIN格式*/
		result = IntelHexFileToBin(ihf, bin_file);
		free(ihf);
	}
	if (top == 1)
	{
		free(bin_file);
	}
	lua_pushboolean(L, result);
	return 1;
}

static int l_HexMerge(lua_State* L)
{
	int top = 0;
	const char* src1 = NULL;
	const char* src2 = NULL;
	const char* dest = NULL;
	IntelHexFormat* ihf1 = NULL;
	IntelHexFormat* ihf2 = NULL;

	top = lua_gettop(L);
	if ((top == 3) && 
		(lua_isstring(L, -1) == 1) && 
		(lua_isstring(L, -2) == 1) &&
		(lua_isstring(L, -3) == 1))
	{
		src1 = lua_tostring(L, -3);
		src2 = lua_tostring(L, -2);
		dest = lua_tostring(L, -1);
	}
	else
	{
		luaL_error(L, "Wrong Arguments.");
		return 0;
	}
	ihf1 = IntelHexFileInput(src1);
	if (ihf1 == NULL)
	{
		return 0;
	}
	ihf2 = IntelHexFileInput(src2);
	if (ihf2 == NULL)
	{
		return 0;
	}
    printf("Merging hex file %s and %s...\n", src1, src2);
	if (IntelHexFileMerge(ihf1, ihf2) == TRUE)
	{
		IntelHexFileOutput(ihf1, dest);
	}
	free(ihf1);
	free(ihf2);
	lua_pushboolean(L, TRUE);
	return 1;
}

static const luaL_reg IntelHexFunctions[]=
{
	{"HexToBin",l_HexToBin},
	{"HexMerge",l_HexMerge},
	{NULL,NULL}
};

int __declspec(dllexport) luaopen_intelhex(lua_State* L)
{
	luaL_openlib(L,"intelhex",IntelHexFunctions,0);
    printf("Library intelhex V1.00.\n");
	return 1;
}
