#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "math.h"

static  int  l_sin(lua_State  *L)  {
    double  d  =  luaL_checknumber(L,  1);
    lua_pushnumber(L,  sin(d));
	printf("l_sin called!\n");
    return  1;    /*  number  of  results  */
}

static  const  struct  luaL_Reg  mylib[]  =  {
	{"sine",  l_sin},
	{NULL,  NULL}    /*  sentinel  */
};

__declspec(dllexport) int  luaopen_mylib  (lua_State  *L)  {
	luaL_register(L,  "mylib",  mylib);
	return  1;
}
