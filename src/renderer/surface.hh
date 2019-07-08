#pragma once

#include <GLFW/glfw3.h>

#include <cstdio>
#include <gsl/gsl>
#include <memory>
#include <string>

#include "render_device_vk.hh"

namespace CEngine {
class surface {
public:
  virtual void initialize(int width, int height)                   = 0;
  virtual void terminate()                                         = 0;
  virtual void show()                                              = 0;
  virtual void hide()                                              = 0;
  virtual void set_render_device(const CEngine::render_device_vk&) = 0;

protected:
};

class glfw_vk_surface final : public surface {
protected:
  std::shared_ptr<CEngine::render_device_vk> renderer;
  int                                        width;
  int                                        height;
  std::string                                title;
  GLFWwindow*                                wnd;

public:
  ~glfw_vk_surface() {
    renderer->terminate();
  }

  virtual void initialize(int width = 1920, int height = 1080) override {
    this->width  = width;
    this->height = height;
    // initialize glfw
    glfwSetErrorCallback(&(error_callback));
    int init = glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  }
  virtual void terminate() override {
    if (wnd)
      glfwDestroyWindow(wnd);
    glfwTerminate();
  }
  virtual void show() override {
    this->wnd =
        glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    glfwShowWindow(wnd);
    Expects(wnd);
    glfwSetKeyCallback(wnd, key_callback);
    glfwSetWindowCloseCallback(wnd, close_window_callback);
    /* glfwMakeContextCurrent(wnd); */

    renderer->initialize();
    while (!glfwWindowShouldClose(wnd)) {
      /* renderer.render(); */
			// swap buffer
      glfwPollEvents();
    }
  }
  virtual void hide() override {}
  virtual void
  set_render_device(const CEngine::render_device_vk& renderer) override {
    this->renderer = std::make_shared<CEngine::render_device_vk>(renderer);
  }
  static void error_callback(int error_code, const char* error_desc) {
    printf("GLFW error %d: %s", error_code, error_desc);
  }
  static void key_callback(GLFWwindow* wnd, int key, int scancode, int action,
                           int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
      glfwSetWindowShouldClose(wnd, GLFW_TRUE);
  }
  static void close_window_callback(GLFWwindow* wnd) {
    glfwSetWindowShouldClose(wnd, GLFW_TRUE);
  }
};
} // namespace CEngine
