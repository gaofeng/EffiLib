
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

	top = lua_gettop(L);
	if (top == 1)
	{
		hex_file = luaL_checkstring(L, 1);
		str_len = strlen(hex_file);
		if (strcmp(hex_file + str_len - 4, ".hex") == 0)
		{
			bin_file = malloc(str_len + 1);
			if (bin_file == NULL)
			{
				printf("Error: Not enough memory.\n");
                lua_pushboolean(L, FALSE);
                return 1;
			}
			memset(bin_file, 0x00, str_len + 1);
			strcpy(bin_file, hex_file);
			memcpy(bin_file + str_len - 4, ".bin", 4);
		}
	}
	else if (top == 2)
	{
		hex_file = luaL_checkstring(L, 1);
		bin_file = (char*)luaL_checkstring(L, 2);
	}
	else
	{
		luaL_error(L, "Must have one or two arguments.");
        lua_pushboolean(L, FALSE);
        return 1;
	}

	printf("读入HEX文件: %s\n", hex_file);
	ihf = IntelHexFileInput(hex_file);
	if (ihf)
	{
		/*数据间隙用0xFF填充*/
		IntelHexFileFillEmptyValue(ihf, 0xFF);
		printf("输出BIN格式文件: %s\n", bin_file);
		if (IntelHexFileToBin(ihf, bin_file) == TRUE)
		{
			lua_pushboolean(L, TRUE);
		}
		else
		{
			printf("ERROR: 输出BIN格式文件失败。\n");
			lua_pushboolean(L, FALSE);
		}
	}
	else
	{
		printf("ERROR: 读取HEX文件失败。\n");
		lua_pushboolean(L, FALSE);
	}
	if (top == 1)
	{
		free(bin_file);
	}
	IntelHexFree(ihf);
	return 1;
}

static int l_HexMerge(lua_State* L)
{
	const char* src1 = NULL;
	const char* src2 = NULL;
	const char* dest = NULL;
	IntelHexFormat* ihf1 = NULL;
	IntelHexFormat* ihf2 = NULL;

	src1 = luaL_checkstring(L, 1);
	src2 = luaL_checkstring(L, 2);
	dest = luaL_checkstring(L, 3);

	fprintf(stdout, "读入第一个源Intelhex文件: %s\n", src1);
	ihf1 = IntelHexFileInput(src1);
	if (ihf1 == NULL)
	{
        lua_pushboolean(L, FALSE);
        return 1;
	}
	fprintf(stdout, "读入第二个源Intelhex文件: %s\n", src2);
	ihf2 = IntelHexFileInput(src2);
	if (ihf2 == NULL)
	{
        lua_pushboolean(L, FALSE);
		return 1;
	}
	if (IntelHexFileMerge(ihf1, ihf2) == TRUE)
	{
		if (IntelHexFileOutput(ihf1, dest) == TRUE)
		{
			fprintf(stdout, "合并后的HEX文件为：%s\n", dest);
			lua_pushboolean(L, TRUE);
		}
		else
		{
			fprintf(stdout, "ERROR: 输出合并后HEX文件时发生错误。\n");
			lua_pushboolean(L, FALSE);
		}
	}
	else
	{
		fprintf(stdout, "ERROR: 合并HEX文件时发生错误。\n");
		lua_pushboolean(L, FALSE);
	}
	IntelHexFree(ihf1);
	IntelHexFree(ihf2);
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
    printf("Library intelhex V1.04.\n");
	return 1;
}
