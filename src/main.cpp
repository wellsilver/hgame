#include <iostream>
#include <string>

#ifdef _WIN32
  #include "plat/win32.hpp"
#elif
  #error not implemented
#endif

#include "clutil.hpp"

void entry() {
  size_t threads;
  
  while (!closing) {
    framebufferwait();

    threads = screenx*screeny;
    clSetKernelArg(shader, 0, sizeof(cl_mem), framebuffer);
    clEnqueueNDRangeKernel(devicequeue, shader, 1, NULL, &threads, NULL, 0, NULL, NULL);
    clFinish(devicequeue);

    framebufferfree();
    framebufferrender();
  }
}