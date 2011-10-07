#include <windows.h>

void ExecWinExec(LPCSTR cmd);

DWORD ExecCreateProcess(const char* command_str, char** result);