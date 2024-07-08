#ifndef win32h
#define win32h

#ifndef UNICODE
#define UNICODE
#endif 

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/opencl.h>

#include "../shader.hpp"

#include <sstream>
#include <thread>

#include <windows.h>
#include <stdio.h>

const wchar_t *windowname = L"Hgame";

void entry();

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND hwndsave;

bool closing = false;

struct rgb {
  unsigned char r,g,b;
};

unsigned int screenx = 0;
unsigned int screeny = 0;
cl_mem mainbuffer = 0;
struct rgb *externalbuffer = 0;

cl_device_id devicecl;
cl_command_queue devicequeue;
cl_context devicecontext;
cl_kernel shader;

void mainbufferrender() {
  PAINTSTRUCT ps;

  HDC hdc = BeginPaint(hwndsave, &ps);
  
  clFinish(devicequeue);
  cl_int err = clEnqueueReadBuffer(devicequeue, mainbuffer, CL_TRUE, 0, (screenx*screeny)*sizeof(struct rgb), externalbuffer, 0, NULL, NULL);
  printf("err: %i\n", err);
  printf("a: %u\n", externalbuffer[0].r);

  EndPaint(hwndsave, &ps);

  Sleep(1000/1);
}

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

  cl_int err;
  cl_platform_id plat;
  clGetPlatformIDs(1, &plat, nullptr);
  clGetDeviceIDs(plat, CL_DEVICE_TYPE_GPU, 1, &devicecl, nullptr);
  devicecontext = clCreateContext(nullptr, 1, &devicecl, nullptr, nullptr, &err);
  if (err!=CL_SUCCESS) return err;
  devicequeue = clCreateCommandQueueWithProperties(devicecontext, devicecl, 0, &err);
  if (err!=CL_SUCCESS) return err;

  char name[32];
  clGetDeviceInfo(devicecl, CL_DEVICE_NAME, 31, name, NULL);
  name[31] = 0; // null terminator
  printf("Via: %s\n", name);

  // Create the compute program from the source buffer
  size_t length = shader_code.size();
  char *strings[1] = {shader_code.data()};

  cl_program program = clCreateProgramWithSource(devicecontext, 1, (const char **) strings, &length, &err);
  if (err!=CL_SUCCESS) return err;

  // Build the program  
   err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
  if (err != CL_SUCCESS) {
    size_t len;
    char buffer[2048];

    printf("Compiling failed.\n");
    clGetProgramBuildInfo(program, devicecl, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
    printf("%s\n", buffer);
  }

  // Create the compute kernel from the program 
  shader = clCreateKernel(program, "pixel", &err);
  if (err!=CL_SUCCESS) return err;

  ShowWindow(hwnd, nCmdShow);

  hwndsave = hwnd; // so early_exit can send its message

  // its in a thread because most of windows isnt thread safe
  // ^ that sentence makes sense, fr fr. Its because I need to put the loop in the windows message loop
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
    case WM_SIZE: // resize the mainbuffer so it fits
      screenx = LOWORD(lParam);
      screeny = HIWORD(lParam);

      if (mainbuffer!=0) {
        clReleaseMemObject(mainbuffer);
        delete externalbuffer;
      }
      externalbuffer = new struct rgb[(screenx*screeny)];
      cl_int err;
      mainbuffer = clCreateBuffer(devicecontext, CL_MEM_WRITE_ONLY, (screenx*screeny) * sizeof(struct rgb), NULL, &err);

      printf("err: %i, mainbuffer: %i\n", err, mainbuffer);
      if (err != CL_SUCCESS) {printf("opencl mainbuffer error %i\n",err);PostMessage(hwnd, WM_DESTROY, 0, 0);};
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

#endif