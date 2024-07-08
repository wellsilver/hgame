#include <iostream>
#include <string>

#ifdef _WIN32
  #include "plat/win32.hpp"
#elif
  #error "not implemented"
  // if its right or not it will give an error as intended lmao
#endif

void entry() {
  cl_int err;
  while (!closing) {
    
    size_t threads[3] = {1,1,1};
    printf("mainbuffer %i\n", mainbuffer);
    err = clSetKernelArg(shader, 0, sizeof(cl_mem), mainbuffer);
    if (err != CL_SUCCESS) {earlyexit(std::string("opencl error ") + std::to_string(err));break;};

    err = clEnqueueNDRangeKernel(devicequeue, shader, 1, NULL, threads, NULL, 0, NULL, NULL);
    if (err != CL_SUCCESS) {earlyexit(std::string("opencl error ") + std::to_string(err));break;};

    mainbufferrender();

  }
}