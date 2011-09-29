#include <stdio.h>
#include "exec.h"

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

DWORD ExecCreateProcess(WCHAR* command_str)
{
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    HANDLE hStdout;
    SECURITY_ATTRIBUTES sa;
    HANDLE hRead,hWrite;
    char buffer[4096] = {0};
    DWORD bytesRead;
    DWORD ExitCode = ERROR;

    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;
    if (!CreatePipe(&hRead,&hWrite,&sa,0)) {
        printf("Error On CreatePipe()");
        return ERROR;
    }

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    ZeroMemory(&si, sizeof(si));
    si.cb=sizeof(si);
    si.wShowWindow=SW_SHOW;
    si.dwFlags=STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    si.hStdOutput = hWrite;

    if (CreateProcess(NULL, command_str, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
    {
        WaitForSingleObject(pi.hProcess, INFINITE);
        GetExitCodeProcess(pi.hProcess, &ExitCode);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }
    else
    {
        MessageBox(NULL, L"The process could not be started...", NULL, MB_OK);
    }
    CloseHandle(hWrite);


    while (TRUE) 
    {
        if (ReadFile(hRead,buffer,4095,&bytesRead,NULL) == TRUE)
        {
            printf(buffer);
            break;
        }
    }
    return ExitCode;
}
