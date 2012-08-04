// Generic - Win32 程序的基础写法

#include <windows.h>
#include "resource.h" //内含各个 resource IDs
#include "generic.h"

HINSTANCE _hInst;	// Instance handle
HWND 	  _hWnd;

char _szAppName[] = "Generic";	//程序名称
char _szTitle[]   = "Generic Sample Application";	//窗口标题
/*
this message loop retrieves messages from the thread's message queue and 
dispatches them to the appropriate window procedures.
*/
// WinMain - 程序入口点
int CALLBACK WinMain(HINSTANCE hInstance, 
					 HINSTANCE hPrevInstance,	//handle to the previous instance of the application. Is always NULL.
					 LPSTR lpCmdLine, 
					 int nCmdShow)	//Controls how the windows is to be shown.
{
	MSG msg;
	WNDCLASS wc;
	UNREFERENCED_PARAMETER(lpCmdLine);	// Avoid complile warning.

	if (!hPrevInstance)
	{
		//Init Application
		// 注册窗口类别
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = (WNDPROC)WndProc;	//窗口函数
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = NULL;//LoadIcon(hInstance, "jjhouricon");
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = GetStockObject(WHITE_BRUSH);	//窗口背景颜色
		wc.lpszMenuName = "GenericMenu";
		wc.lpszClassName = _szAppName;

		if (!RegisterClass(&wc))
		{
			return FALSE;
		}
	}

	_hInst = hInstance; // 存储为全域变量，方便使用

	// Init Instance
	// Create the main window
	_hWnd = CreateWindow(
		_szAppName,
		_szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
		);
	// If the main window cannot be created, terminate 
    // the application.
	if (!_hWnd)
		return (FALSE);

	//Show the window and paint its contents.
	ShowWindow(_hWnd, nCmdShow);	//显示窗口
	UpdateWindow(_hWnd);			//送出 WM_PAINT

	// Return zero when retrives the WM_QUIT message.
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		//Translates virtual-key messages into character messages.
		TranslateMessage(&msg);
		//Dispatches a message to a window procedure.
		DispatchMessage(&msg);
	}

	return (msg.wParam); // 传回 PostQuitMessage 的参数
}

//---------------------------------------------------------------
// WndProc - 
//---------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
						 WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message) {
		case WM_COMMAND:
			wmId    = LOWORD(wParam);
			wmEvent = HIWORD(wParam);

			switch (wmId) {
				case IDM_ABOUT:
 					DialogBox(_hInst,         // Winmain第一个参数，应用程序的实例句柄
                    	      "AboutBox",     // 对话框资源名称
                        	  hWnd,           // 父窗口
                        	  (DLGPROC)About  //对话框函数名称
                         	 );
                	break;
            	case IDM_EXIT:
	                // 使用者想结束程序。处理方式与 WM_CLOSE 相同
    	            DestroyWindow (hWnd);
        	        break;
            	default:
                	return (DefWindowProc(hWnd, message, wParam, lParam));
        	}
        	break;

      	case WM_DESTROY:  // 窗口已经被摧毀 (程序即将结束) 。
        	PostQuitMessage(0);
        	break;
      	default:
        	return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return (0);
}

//---------------------------------------------------------------
// About -对话框函数
//---------------------------------------------------------------
LRESULT CALLBACK About(HWND hDlg, UINT message,
  					   WPARAM wParam, LPARAM lParam)
{
  	UNREFERENCED_PARAMETER(lParam);      // 避免編译时的警告

    switch (message) {
    	case WM_INITDIALOG:
            return (TRUE);      // TRUE 表示我已处理过这个消息

        case WM_COMMAND:
            if ((LOWORD(wParam) == IDOK) || 
            	(LOWORD(wParam) == IDCANCEL))
            {
            	EndDialog(hDlg, TRUE);
                return (TRUE);  // TRUE 表示我已处理过这个消息
            }
            break;
    }
 	return (FALSE); // FALSE 表示我没有处理这个消息
}

