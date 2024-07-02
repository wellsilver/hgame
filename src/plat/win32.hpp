#ifndef win32h
#define win32h

#ifndef UNICODE
#define UNICODE
#endif 

#include <sstream>
#include <thread>

#include <windows.h>
#include <stdio.h>

const wchar_t *windowname = L"Hgame";

void entry();

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND hwndsave;

bool closing = false;
bool windowresized = false;

unsigned int screenx = 0;
unsigned int screeny = 0;

void earlyexit(std::string str) {
  PostMessage(hwndsave, WM_DESTROY, 0, 0);
  std::wstring wstr; for (auto i : str) wstr += i;
  MessageBox(NULL, wstr.c_str(), windowname, MB_ICONERROR);
}

void crashandburn() {
  PostMessage(hwndsave, WM_DESTROY, 0, 0);
  MessageBox(hwndsave, L"Crashed", windowname, MB_ICONERROR);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
#ifndef debug
  std::set_terminate(crashandburn);
#endif
  // Register the window class.
  const wchar_t CLASS_NAME[]  = L"Sample Window Class";
  
  WNDCLASS wc = {};

  wc.lpfnWndProc   = WindowProc;
  wc.hInstance     = hInstance;
  wc.lpszClassName = CLASS_NAME;

  RegisterClass(&wc);

  // Create the window.

  HWND hwnd = CreateWindowEx(
    0,                              // Optional window styles.
    CLASS_NAME,                     // Window class
    windowname,    // Window text
    WS_OVERLAPPEDWINDOW,            // Window style

    // Size and position
    CW_USEDEFAULT, CW_USEDEFAULT, 480, 480,

    NULL,       // Parent window    
    NULL,       // Menu
    hInstance,  // Instance handle
    NULL        // Additional application data
  );

  if (hwnd == NULL) return -1;

  ShowWindow(hwnd, nCmdShow);

  hwndsave = hwnd; // so early_exit can send its message

  // its in a thread because most of windows isnt thread safe
  // ^ that sentence makes sense, fr fr. Its because I need to put the message thing in some loop which.. nah
  std::thread entrythread(entry);

  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  entrythread.join();

  return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_DESTROY:
      closing = true;
      PostQuitMessage(0);
      return 0;
    case WM_PAINT:
      return 0;
    case WM_SIZE:
      windowresized = true;
      screenx = LOWORD(lParam);
      screeny = HIWORD(lParam);
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

#endif