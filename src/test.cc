#include <iostream>

#include "renderer/renderer.hh"

int main(int argc, char* argv[]){
  std::cout << "HELLO" << std::endl;
	CEngine::glfw_vk_surface surface{};
	surface.initialize();
	CEngine::render_device_vk render_device{};
	surface.set_render_device(render_device);
	surface.show();
	surface.terminate();
  return 0;
}
