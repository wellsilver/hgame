#include <iostream>
#include <string>

#ifdef _WIN32
  #include "plat/win32.hpp"
#endif

#include "clutil.hpp"

void entry() {
  firstdevi();

  size_t threads = 1;
  clEnqueueNDRangeKernel(devicequeue, shader, 1, NULL, &threads, NULL, 0, NULL, NULL);
  clFinish(devicequeue);
}