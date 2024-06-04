// completely wrap glfw and be a renderer for vulkan
#ifndef vulkanhpp
#define vulkanhpp

#include <string>

namespace vkl {
  namespace ext {
    #include <vulkan/vulkan.h>
    #include <GLFW/glfw3.h>
  }

  void init() {
    ext::glfwInit();

    ext::glfwInitVulkanLoader(ext::vkGetInstanceProcAddr);

    // combust if doesnt support vulkan
    if (!ext::glfwVulkanSupported()) throw(std::string("No Vulkan Support"));
  }

  class window {
  public:
    ext::GLFWwindow *vwindow;
    bool wantsclose = false;

    enum windowstyle {
      // fullscreen in resolution sizex, sizey
      fullscreen=1,
      // fullscreen in monitor's resolution
      autofullscreen=2,
      // a window in resolution sizex, sizey
      windowed=3,
      // a window automatically to a fourth of the screen's size (1:1 aspect) 
      fourthwindowed=4
    };
    // open window fullscreen
    window(int sizex, int sizey, enum windowstyle style, std::string name) {
      ext::GLFWmonitor *f = ext::glfwGetPrimaryMonitor();
      const ext::GLFWvidmode *v = ext::glfwGetVideoMode(f);

      switch (style) {
        case windowstyle::fullscreen:
          ext::glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
          ext::glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
          ext::glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
          vwindow = ext::glfwCreateWindow(sizex, sizey, name.c_str(), f, nullptr);
        break;
        case windowstyle::autofullscreen:
          ext::glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
          ext::glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
          ext::glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
          vwindow = ext::glfwCreateWindow(v->width, v->height, name.c_str(), f, nullptr);
        break;
        case windowstyle::windowed:
          ext::glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
          ext::glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
          vwindow = ext::glfwCreateWindow(sizex, sizey, name.c_str(), f, nullptr);
        break;
        case windowstyle::fourthwindowed:
          ext::glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
          ext::glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
          if (v->height < v->width) { // hopefully this ensures its square lol
            vwindow = ext::glfwCreateWindow(v->height/4, v->height/4, name.c_str(), nullptr, nullptr);
          } else {
            vwindow = ext::glfwCreateWindow(v->width/4, v->width/4, name.c_str(), nullptr, nullptr);
          }
        break;
      }

    }

    void poll() {
      ext::glfwPollEvents();

      wantsclose = ext::glfwWindowShouldClose(vwindow);
    }

    void wait() {
      ext::glfwWaitEvents();

      wantsclose = ext::glfwWindowShouldClose(vwindow);
    }

    void destroy() {
      ext::glfwDestroyWindow(vwindow);

      ext::glfwTerminate();
    }
  };

  // a gfx renderer, for creating menu's and stuff like imgui
  class gfx {
  public:

    gfx(ext::GLFWwindow *window) {
      
    }
    gfx() {
      
    }

  };

  // a 2d renderer
  class rd2 {
  public:

    rd2(ext::GLFWwindow *window) {
      
    }
    rd2() {
      
    }

  };

  // a 3d renderer
  class rd3 {
  public:

    rd3(ext::GLFWwindow *window) {

    }
    rd3() {

    }

  };
}


#endif