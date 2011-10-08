#include <stdio.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "exec.h"
#include "stringbuffer.h"

void ExecWinExec(LPCSTR cmd)
{
    UINT result = 0;
    result = WinExec(cmd, SW_HIDE);
    switch(result)
    {
        case 0:
            printf("Execute success!\n");
            break;
        case ERROR_BAD_FORMAT:
            printf("Bad format!\n");
            break;
        case ERROR_FILE_NOT_FOUND:
            printf("File not found!\n");
            break;
        case ERROR_PATH_NOT_FOUND:
            printf("Path not fouond!\n");
            break;
        case ERROR_LOCK_VIOLATION:
            printf("Lock voilation!\n");
            break;
        default:
            printf("Unkown Error!\n");
            break;
    }
}

DWORD ExecCreateProcess(const char* command_str, char** result)
{
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    //HANDLE hStdout;
    SECURITY_ATTRIBUTES sa;
    HANDLE hRead,hWrite;
    char buffer[4096] = {0};
    DWORD bytesRead;
    DWORD ExitCode = ERROR;
	char* runcmd;

    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;
    if (!CreatePipe(&hRead,&hWrite,&sa,0))
	{
        printf("Error On CreatePipe()");
        return ERROR;
    }

	runcmd=StringInit();
	runcmd=StringSet(runcmd, command_str);
	//runcmd=StringInsert (runcmd,"c://windows//system32//cmd.exe /c ",0);

    //hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.wShowWindow = SW_SHOW;
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.hStdOutput = hWrite;
	si.hStdError = hWrite;

    if (CreateProcess(NULL, runcmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
    {
		printf("Excuting \"%s\"\n", runcmd);
        WaitForSingleObject(pi.hProcess, INFINITE);
        GetExitCodeProcess(pi.hProcess, &ExitCode);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }
    else
    {
        //MessageBox(NULL, "The process could not be started...", NULL, MB_OK);
		printf("The process could not be started.\n");
		CloseHandle(hWrite);
		free(runcmd);
		return 0;
    }
    CloseHandle(hWrite);
	*result = StringInit();

    while (TRUE) 
    {
        if (ReadFile(hRead, buffer, 4095, &bytesRead, NULL) == FALSE)
        {
            break;
        }
		
		*result = StringAppent(*result, buffer);
		memset(buffer,0,4096);
		//printf(buffer);
    }

    return ExitCode;
}

static int l_ExecCreateProcess(lua_State* L)
{
	int top = 0;
	const char* cmd = NULL;
	char* result = NULL;
	DWORD ret = 0;

	top = lua_gettop(L);
	if ((top == 1) && (lua_isstring(L, -1) == 1))
	{
		cmd = lua_tostring (L,-1);
	}
	else
	{
		luaL_error(L, "The first argument must be a command string.");
		return 0;
	}

	ret = ExecCreateProcess(cmd, &result);

	//·µ»Ø½á¹û
	lua_pushstring(L, result);
	free(result);
	return 1;
}

static const luaL_reg ExecFunctions[]=
{
	{"ExecCP",l_ExecCreateProcess},
	{NULL,NULL}
};

int __declspec(dllexport) luaopen_exec(lua_State* L)
{
	luaL_openlib(L,"exec",ExecFunctions,0);
	return 1;
}
