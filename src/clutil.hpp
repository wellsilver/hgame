#ifndef clutil_hpp
#define clutil_hpp

#include <iostream>

#include "shader.hpp"

/*
void firstdevi() {
  cl_platform_id platform;
  unsigned int platforms;
  unsigned int devices;
  int err;

  clGetPlatformIDs(1, &platform, &platforms);
  clGetDeviceIDs(platform, CL_DEVICE_TYPE_DEFAULT, 1, &devicecl, &devices);
  devicecontext = clCreateContext(0, 1, &devicecl, NULL, NULL, &err);
  devicequeue = clCreateCommandQueueWithProperties(devicecontext, devicecl, 0, &err);

  // Create the compute program from the source buffer
  size_t length = shader_code.size();
  char *strings[1] = {shader_code.data()};

  cl_program program = clCreateProgramWithSource(devicecontext, 1, (const char **) strings, &length, &err);

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
}
*/

#endif