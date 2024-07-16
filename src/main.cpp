#include <iostream>
#include <string>
#include <windows.h>
#define CL_TARGET_OPENCL_VERSION 120
#include <CL/cl.h>
#include "shader.hpp"

bool closing = false;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
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
    L"Hgame",    // Window text
    WS_OVERLAPPEDWINDOW,            // Window style

    // Size and position
    CW_USEDEFAULT, CW_USEDEFAULT, 480, 480,

    NULL,       // Parent window    
    NULL,       // Menu
    hInstance,  // Instance handle
    NULL        // Additional application data
  );

  if (hwnd == NULL) return -1;
  
  cl_command_queue queue;
  cl_kernel kernel;
  {
    cl_int err;
    cl_platform_id platform;
    clGetPlatformIDs(1, &platform, NULL);
    cl_device_id device;
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, &device, NULL);

    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    if (err!=CL_SUCCESS) {printf("cl %i\n", err);return -1;}
    cl_command_queue_properties prop = {};
    queue = clCreateCommandQueue(context, device, prop, &err);
    if (err!=CL_SUCCESS) {printf("cl %i\n", err);return -1;}
    size_t length = strlen(shader_code);
    const char **shadersrc = &shader_code;
    cl_program prgm = clCreateProgramWithSource(context, 1, shadersrc, &length, &err);
    if (err!=CL_SUCCESS) {printf("cl %i\n", err);return -1;}
    err = clBuildProgram(prgm, 0, NULL, NULL, NULL, NULL);
    if (err!=CL_SUCCESS) {printf("cl %i\n", err);return -1;}
    kernel = clCreateKernel(prgm, "pixel", &err);
    if (err!=CL_SUCCESS) {printf("cl %i\n", err);return -1;}
  }

  ShowWindow(hwnd, nCmdShow);

  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
    case WM_PAINT:
      return 0;
    case WM_SIZE:
      return 0;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}