#include <stdio.h>

#include "exec.h"

int main(void)
{
    //WCHAR command_string[100];
    DWORD exit_code;
    //wcscpy(command_string, L"notepad.exe");
    //ExecWinExec("ping 10.5.1.1 -t");
	char* result = NULL;
    exit_code = ExecCreateProcess("C:\\Program Files\\Git\\bin\\git.exe", &result);
	printf(result);
}