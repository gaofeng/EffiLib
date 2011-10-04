#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"


void  error(lua_State  *L,  const  char  *fmt,  ...)  
{
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	fprintf(stderr, "\n");
	va_end(argp);
	lua_close(L);
	exit(EXIT_FAILURE);
}

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

void  loadWindowConfig(lua_State  *L,  const  char  *fname,  int  *w,  int  *h)  {
    if  (luaL_loadfile(L,  fname)  ||  lua_pcall(L,  0,  0,  0))
    {
        error(L,  "cannot run config file:  %s",  lua_tostring(L,  -1));
    }
    lua_getglobal(L,  "width");
    lua_getglobal(L,  "height");
    if  (!lua_isnumber(L,  -2))
    {
        error(L,  "¡¯width¡¯  should  be  a  number\n");
    }
    if  (!lua_isnumber(L,  -1))
    {
        error(L,  "¡¯height¡¯  should  be  a  number\n");
    }
    *w  =  lua_tointeger(L,  -2);
    *h  =  lua_tointeger(L,  -1);
}

#define MAX_COLOR 255

struct  ColorTable
{
	char  *name;
	unsigned  char  red,  green,  blue;
} colortable[] =
{
	{"WHITE", MAX_COLOR, MAX_COLOR, MAX_COLOR},
	{"RED",   MAX_COLOR, 0,			0},
	{"GREEN", 0,		 MAX_COLOR, 0},
	{"BLUE",  0,		 0,			MAX_COLOR},
	{NULL,	  0,         0,         0}    /*  sentinel  */
};

/*  assume  that  table  is  on  the  stack  top  */
int  getfield(lua_State *L, const char *key)  {
	int result;
	lua_pushstring(L, key);
	lua_gettable(L, -2);    /*  get  background[key]  */
	if (!lua_isnumber(L, -1))
	{
		error(L, "invalid component in background color");
	}
	result = (int)(lua_tonumber(L,  -1) * MAX_COLOR);
	lua_pop(L, 1);    /*  remove  number  */
	return result;
}

/*  assume  that  table  is  at  the  top  */
void  setfield(lua_State  *L, const char *index, int value)  {
	lua_pushnumber(L, (double)value / MAX_COLOR);
	lua_setfield(L, -2, index);
}

void  setcolor(lua_State *L, struct ColorTable *ct)  {
	lua_newtable(L); /*  creates  a  table  */
	setfield(L, "r", ct->red); /*  table.r  =  ct->r  */
	setfield(L, "g", ct->green);         /*  table.g  =  ct->g  */
	setfield(L, "b", ct->blue); /*  table.b  =  ct->b  */
	lua_setglobal(L, ct->name); /*  ¡¯name¡¯  =  table  */
}

void TableTest(lua_State *L)
{
	int i = 0;
	unsigned char red, green, blue;
	/*Set Predefined color that can use in lua scripts*/
	while (colortable[i].name != NULL)
	{
		setcolor(L, &colortable[i++]);
	}
	/*Put "backgroud" variable at stack top*/
	lua_getglobal(L, "background");
	/*  value  is  a  string?  */
	if (lua_isstring(L, -1))
	{
		/*  get  string  */
		const char *colorname = lua_tostring(L, -1);    
		int i;
		/*  search  the  color  table  */
		for (i = 0; colortable[i].name != NULL; i++)  
		{
			if (strcmp(colorname, colortable[i].name) == 0)
			{
				break;
			}
		}
		if (colortable[i].name == NULL)
		{
			/*  string  not  found?  */
			error(L,  "invalid  color  name  (%s)",  colorname);
		}
		else  
		{    
			/*  use  colortable[i]  */
			red   = colortable[i].red;
			green = colortable[i].green;
			blue  = colortable[i].blue;
		}
	} 
	else if (lua_istable(L,  -1))  
	{
		red   = getfield(L, "r");
		green = getfield(L, "g");
		blue  = getfield(L, "b");
	}  
	else
	{
		error(L, "invalid value for 'background'");
	}
	printf("Background color is: r(%d)-g(%d)-b(%d)\n", red, green, blue);
}

int  main(void)  
{
    int w = 0;
    int h = 0;

    lua_State  *L  =  luaL_newstate(); /*  opens  Lua  */
    luaL_openlibs(L); /*  opens  the  standard  libraries  */

    /*Run Lua Simple Interpreter*/
    //RunInterpreter(L);
    loadWindowConfig(L, "win_size.lua", &w, &h);
	TableTest(L);

    lua_close(L);
    return  0;
}