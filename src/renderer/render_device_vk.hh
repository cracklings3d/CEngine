#include "render_device.hh"
#include <vulkan/vulkan.hpp>

#include <cstdio>
#include <gsl/gsl>
#include <vector>

namespace CEngine {
class render_device_vk : public render_device {
public:
  render_device_vk() {
    create_instance();
    create_device();
  }

  virtual ~render_device_vk() override {}

  virtual void initialize() override { }
  virtual void flush() override {}

  virtual void terminate() override {
    if (device) device.destroy();
    if (instance) instance.destroy();
  }

  virtual void render(const renderable &item) const override {}

protected:
  vk::Instance instance;
  vk::Device   device;

  void create_instance() {
    vk::ApplicationInfo app_info{};
    app_info.apiVersion         = VK_API_VERSION_1_1;
    app_info.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    app_info.engineVersion      = VK_MAKE_VERSION(0, 1, 0);
    app_info.pEngineName        = "CEngine";
    app_info.pApplicationName   = "CENGINE_TEST";

    std::vector<const char*> debug_layer_name{"VK_LAYER_LUNARG_core_validation"};
    vk::InstanceCreateInfo   instance_create_info{};
    instance_create_info.pApplicationInfo    = &app_info;
    instance_create_info.enabledLayerCount   = debug_layer_name.size();
    instance_create_info.ppEnabledLayerNames = &debug_layer_name[0];

    instance = vk::createInstance(instance_create_info);
  }

  void create_device() {
    std::vector<vk::PhysicalDevice> physical_devices
        = instance.enumeratePhysicalDevices();

    for (auto pd : physical_devices) {
      vk::PhysicalDeviceProperties pd_property = pd.getProperties();
      if (pd_property.vendorID == 4318 // nvidia
      ) {
        auto pd_queue_family_properties = pd.getQueueFamilyProperties();

        vk::DeviceCreateInfo device_create_info = {};

        return;
      }
    }
    printf(
      "Found no GPU with Vulkan support!"
      " Check your Graphics card and driver.");
  }
};

} // namespace CEngine
