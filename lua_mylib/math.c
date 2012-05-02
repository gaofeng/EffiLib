#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "math.h"

#define key "math"

static void stackDump(lua_State *L)  
{
	int i;
	int top = lua_gettop(L);
	for (i = 1; i <= top; i++)  
	{    /*  repeat  for  each  level  */
		int t = lua_type(L,  i);
		switch (t)  
		{
			case LUA_TSTRING:      /*  strings  */
				printf("\"%s\"",  lua_tostring(L,  i));
				break;
								
			case LUA_TBOOLEAN:      /*  booleans  */
				printf(lua_toboolean(L,  i)  ?  "true"  :  "false");
				break;
								 
			case LUA_TNUMBER:      /*  numbers  */
				printf("%g",  lua_tonumber(L,  i));
				break;
								
			default:      /*  other  values  */
				printf("%s",  lua_typename(L,  t));
				break;
		}
		printf("    ");    /*  put  a  separator  */
	}
	printf("\n");    /*  end  the  listing  */
}

static  int  l_sin(lua_State  *L)  {
    double  d  =  luaL_checknumber(L,  1);
    int environ_num = 0;
	lua_pushnumber(L,  sin(d));
	printf("l_sin called!\n");
	lua_getfield(L, LUA_ENVIRONINDEX, key);
	if (lua_isnumber(L, -1))
	{
		environ_num = (int)lua_tonumber(L, -1);
		printf("environ: %d\n", environ_num);
		environ_num += 1;
	}
	else
	{
		environ_num = 1;
	}
	lua_pop(L, 1);
	lua_pushnumber(L, environ_num);
	lua_setfield(L, LUA_ENVIRONINDEX, key);
    return  1;    /*  number  of  results  */
}

static  int  l_add(lua_State  *L)  {
    double  a  =  luaL_checknumber(L,  1);
    double  b  =  luaL_checknumber(L,  2);
	int environ_num = 0;
	lua_pushnumber(L,  a + b);
	printf("l_add called!\n");

	lua_getfield(L, LUA_ENVIRONINDEX, key);
	if (lua_isnumber(L, -1))
	{
		environ_num = (int)lua_tonumber(L, -1);;
		printf("environ: %d\n", environ_num);
		environ_num += 1;
	}
	else
	{
		environ_num = 0;
	}
	lua_pop(L, 1);
	lua_pushnumber(L, environ_num);
	lua_setfield(L, LUA_ENVIRONINDEX, key);
    return  1;    /*  number  of  results  */
}

/*在模块内，ENVIRON是共享的*/

static  const  struct  luaL_Reg  mylib[]  =  {
	{"sine",  l_sin},
	{"add",  l_add},
	{NULL,  NULL}    /*  sentinel  */
};

__declspec(dllexport) int  luaopen_mylib  (lua_State  *L)  {
	luaL_register(L,  "mylib",  mylib);
	return  1;
}
