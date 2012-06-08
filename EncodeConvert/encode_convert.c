#include"lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include <windows.h>

#define CHAR_SCALE (sizeof(wchar_t)/sizeof(char))

wchar_t* AToU(const char* str)
{
    int textlen;
    wchar_t* result;

    textlen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    result = (wchar_t*)malloc((textlen + 1) * sizeof(wchar_t));
    memset(result, 0, (textlen + 1) * sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, 0, str, -1, (LPWSTR)result, textlen);
    return result;
}

char * UToA(const wchar_t* str)
{
    char * result;
    int textlen;

    // wide char to multi char
    textlen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
    result= (char*)malloc((textlen + 1) * sizeof(char));
    memset(result, 0, sizeof(char) * (textlen + 1));
    WideCharToMultiByte(CP_ACP, 0, str, -1, result, textlen, NULL, NULL);
    return result;
}

wchar_t* U8ToU(const char* str)
{
    int textlen;
    wchar_t* result;

    textlen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    result = (wchar_t*)malloc((textlen + 1) * sizeof(wchar_t));
    memset(result, 0, (textlen + 1) * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)result, textlen);
    return result;
}

char* UToU8(const wchar_t* str)
{
    char* result;
    int textlen;

    // wide char to multi char
    textlen = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
    result = (char*)malloc((textlen + 1) * sizeof(char));
    memset(result, 0, sizeof(char) * (textlen + 1));
    WideCharToMultiByte(CP_UTF8, 0, str, -1, result, textlen, NULL, NULL);
    return result;
}

static int Unicode_a2u (lua_State* L)
{
    const char* str;
    wchar_t* result;

    /*传递第一个参数 */
    str = lua_tostring(L,-1);
    /*开始转换 */
    result=AToU(str);
    /*返回值*/
    lua_pushlstring(L, (char*)result, wcslen(result) * CHAR_SCALE);
    free(result);
    return 1;
}

static int Unicode_u2a(lua_State* L)
{
    const wchar_t* str;
    char* result;

    /*传递第一个参数 */
    str = (wchar_t*)lua_tostring(L,-1);
    /*开始转换 */
    result = UToA(str);
    /*返回值*/
    lua_pushstring(L, result);
    free(result);
    return 1;
}

static int Unicode_u2u8(lua_State* L)
{
    const wchar_t* str;
    char* result;

    /*传递第一个参数 */
    str = (wchar_t*)lua_tostring(L, -1);
    /*开始转换 */
    result = UToU8(str);
    /*返回值， */
    lua_pushstring(L, result);
    free(result);
    return 1;
}

static int Unicode_u82u(lua_State* L)
{
    const char* str;
    wchar_t* result;

    /*传递第一个参数 */
    str = lua_tostring(L, -1);
    /*开始转换 */
    result = U8ToU(str);
    /*返回值， */
    lua_pushlstring(L, (char*)result, wcslen(result) * CHAR_SCALE);
    free(result);
    return 1;
}

static int Unicode_a2u8(lua_State* L)
{
    const char* str;
    wchar_t* temp;
    char* result;

    /*传递第一个参数 */
    str = lua_tostring(L, -1);
    /*开始转换 */
    temp = AToU(str);
    result = UToU8(temp);
    /*返回值， */
    lua_pushstring(L, result);
    free(result);
    return 1;
}

static int Unicode_u82a(lua_State* L)
{
    const char* str;
    wchar_t* temp;
    char* result;

    /*传递第一个参数 */
    str = lua_tostring(L, -1);
    /*开始转换 */
    temp = U8ToU(str);
    result = UToA(temp);
    /*返回值， */
    lua_pushstring(L, result);
    free(result);
    return 1;
}

static const luaL_reg UnicodeFunctions[]=
{
    {"a2u", Unicode_a2u},
    {"u2a", Unicode_u2a},
    {"u2u8", Unicode_u2u8},
    {"u82u", Unicode_u82u},
    {"a2u8", Unicode_a2u8},
    {"u82a", Unicode_u82a},
    {NULL, NULL}
};

int  __declspec(dllexport) luaopen_EncodeConvert(lua_State* L)
{
    luaL_openlib(L, "EncodeConvert", UnicodeFunctions, 0);
    return 1;
}