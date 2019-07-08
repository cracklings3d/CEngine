#include "render_device.hh"
#include <vulkan/vulkan.h>

#include <cstdio>
#include <gsl/gsl>
#include <vector>

namespace CEngine {
class render_device_vk : public render_device {
public:
  render_device_vk() { create_instance(); }
  virtual ~render_device_vk() override {}

  virtual void initialize() override { create_device(); }
  virtual void flush() override {}
  virtual void terminate() override { destroy_instance(); }

  virtual void render(const renderable &item) const override {}

protected:
  VkInstance instance;
  VkDevice   device;

  void create_instance() {
    VkInstanceCreateInfo instance_create_info{};
    instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.enabledLayerCount = 1;
    auto debug_layer_name                  = "VK_LAYER_LUNARG_core_validation";
    instance_create_info.ppEnabledLayerNames = &debug_layer_name;
    VkResult create_instance_result =
        vkCreateInstance(&instance_create_info, nullptr, &this->instance);
    Expects(create_instance_result == VK_SUCCESS);
  }

  void create_device() {
    std::vector<VkPhysicalDevice> physical_devices{};
    uint32_t                      physical_device_count;
    vkEnumeratePhysicalDevices(instance, &physical_device_count, nullptr);
    physical_devices.reserve(physical_device_count);
    vkEnumeratePhysicalDevices(instance, &physical_device_count,
                               &physical_devices[0]);
    // for (int i = 0; i < physical_device_count; i++) {
    //   VkPhysicalDeviceProperties physical_device_property;
    //   vkGetPhysicalDeviceProperties(physical_devices[i],
    //                                 &physical_device_property);
    //   printf("Device %d: %s\n", i, physical_device_property.deviceName);
    //   printf("Device ID:   %d\n", physical_device_property.deviceID);
    //   printf("API version: %d\n", physical_device_property.apiVersion);
    //   printf("Device type: %d\n", physical_device_property.deviceType);
    //   printf("\n");
    // }
    // VkPhysicalDeviceProperties physical_device_property0;
    // vkGetPhysicalDeviceProperties(physical_devices[0],
    // 															&physical_device_property0);
    // VkPhysicalDeviceProperties physical_device_property1;
    // vkGetPhysicalDeviceProperties(physical_devices[1],
    // 															&physical_device_property1);

    for (auto iter = physical_devices.begin(); iter == physical_devices.end();
         iter++) {
      VkPhysicalDeviceProperties physical_device_property;
      vkGetPhysicalDeviceProperties(*iter, &physical_device_property);
      switch (physical_device_property.vendorID) {
        if (physical_device_property.vendorID == 4318 // nvidia
            || physical_device_property.vendorID ==
                   4318 // change to something else
        ) {
          VkDeviceCreateInfo device_create_info = {};
          device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
          VkResult create_device_result =
              vkCreateDevice(*iter, &device_create_info, nullptr, &device);
          Expects(create_device_result == VK_SUCCESS);
          return;
        }
      }
      printf("Found no GPU with Vulkan support! Check your Graphics card and "
             "driver.");
    }
  }
  void destroy_instance() { vkDestroyInstance(instance, nullptr); }
};
} // namespace CEngine
