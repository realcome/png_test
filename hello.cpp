#include <iostream>
#include <mytest.h>


#define MAX_LOADSTRING 100
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

//ATOM MyRegisterClass(HINSTANCE hInstance)
//{
//  WNDCLASSEXW wcex;
//
//  wcex.cbSize = sizeof(WNDCLASSEX);
//
//  wcex.style = CS_HREDRAW | CS_VREDRAW;
//  wcex.lpfnWndProc = WndProc;
//  wcex.cbClsExtra = 0;
//  wcex.cbWndExtra = 0;
//  wcex.hInstance = hInstance;
//  wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
//  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
//  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//  wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
//  wcex.lpszClassName = szWindowClass;
//  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
//
//  return RegisterClassExW(&wcex);
//}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  // TODO: Place code here.

  // Initialize global strings
  //LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  //LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
  //MyRegisterClass(hInstance);

  // Perform application initialization:
  theApp.m_hInstance = hInstance;

  if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
  {
    return FALSE;
  }

  if (!theApp.InitInstance())
  {
    return FALSE;
  }

  MSG msg;

  // Main message loop:
  while (GetMessage(&msg, nullptr, 0, 0))
  {
    if (!TranslateAccelerator(msg.hwnd, NULL, &msg))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return (int)msg.wParam;
}