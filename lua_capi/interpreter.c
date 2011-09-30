#include <stdio.h>
#include <string.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

void RunInterpreter(lua_State* L)
{
    char  buff[256];
    int  error;
    while  (fgets(buff,  sizeof(buff),  stdin)  !=  NULL)  {
        error  =  luaL_loadbuffer(L,  buff,  strlen(buff),  "line")  ||
            lua_pcall(L,  0,  0,  0);
        if  (error)  {
            fprintf(stderr,  "%s",  lua_tostring(L,  -1));
            lua_pop(L,  1);    /*  pop  error  message  from  the  stack  */
        }
    }
}

void  loadWindowConfig(lua_State  *L,  const  char  *fname,  int  *w,  int  *h)  {
    if  (luaL_loadfile(L,  fname)  ||  lua_pcall(L,  0,  0,  0))
    {
        luaL_error(L,  "cannot run config file:  %s",  lua_tostring(L,  -1));
    }
    lua_getglobal(L,  "width");
    lua_getglobal(L,  "height");
    if  (!lua_isnumber(L,  -2))
    {
        luaL_error(L,  "¡¯width¡¯  should  be  a  number\n");
    }
    if  (!lua_isnumber(L,  -1))
    {
        luaL_error(L,  "¡¯height¡¯  should  be  a  number\n");
    }
    *w  =  lua_tointeger(L,  -2);
    *h  =  lua_tointeger(L,  -1);
}


int  main(void)  
{
    int w = 0;
    int h = 0;

    lua_State  *L  =  luaL_newstate(); /*  opens  Lua  */
    luaL_openlibs(L); /*  opens  the  standard  libraries  */

    /*Run Lua Simple Interpreter*/
    RunInterpreter(L);

    //loadWindowConfig(L, "win_size.lua", &w, &h);

    lua_close(L);
    return  0;
}