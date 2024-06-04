#include <iostream>
#include "vulkanlib.hpp"

#include <GLFW/glfw3.h>

int main() {
  try {vkl::init();}
  catch (std::string reason) {
    std::cout << reason << std::endl;
    return -1;
  }

  vkl::window window(0, 0, vkl::window::windowstyle::fourthwindowed, "Dictation");
  
  while (!window.wantsclose) {
    
    
    window.wait();
  }

  window.destroy();
}