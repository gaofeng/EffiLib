#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

void RunInterpreter(lua_State* L)
{
    char buff[256];
    int error;
    while (fgets(buff, sizeof(buff), stdin) != NULL)  {
        error = luaL_loadbuffer(L, buff, strlen(buff),  "line")  ||
            lua_pcall(L,  0,  0,  0);
        if (error)  
		{
            fprintf(stderr,  "%s",  lua_tostring(L,  -1));
            lua_pop(L,  1);    /*  pop  error  message  from  the  stack  */
        }
    }
}



int main(void)  
{
    int w = 0;
    int h = 0;

    lua_State  *L  =  luaL_newstate(); /*  opens  Lua  */
    luaL_openlibs(L); /*  opens  the  standard  libraries  */

    /*Run Lua Simple Interpreter*/
    RunInterpreter(L);

    lua_close(L);
    return  0;
}