#include <stdio.h>

#include "exec.h"

int main(void)
{
    UINT result = 0;
    WCHAR command_string[100];
    DWORD exit_code;
    wcscpy(command_string, L"git.exe");
    //ExecWinExec("ping 10.5.1.1 -t");
    exit_code = ExecCreateProcess(command_string);
}