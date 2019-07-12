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

  virtual ~render_device_vk() override {
    if (device)
      device.destroy();
    if (instance)
      instance.destroy();
	}

  virtual void initialize() override { prepare_pipeline(); }
  virtual void flush() override {}

  virtual void terminate() override {
		if(g_command_pool)
			device.destroyCommandPool(g_command_pool);
  }

  virtual void render(const renderable &item) const override {}

protected:
  vk::Instance       instance;
  vk::PhysicalDevice physical_device;
  vk::Device         device;
  vk::Queue          gqueue;
  vk::Queue          cqueue;
  float              gq_priority = 1;
  float              cq_priority = .5;
  int                gq_family_index;
  int                cq_family_index;
  vk::CommandPool    g_command_pool;

  void create_instance() {
    vk::ApplicationInfo app_info{};
    app_info.apiVersion         = VK_API_VERSION_1_1;
    app_info.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    app_info.engineVersion      = VK_MAKE_VERSION(0, 1, 0);
    app_info.pEngineName        = "CEngine";
    app_info.pApplicationName   = "CENGINE_TEST";

    std::vector<const char *> debug_layer_name{
        "VK_LAYER_LUNARG_core_validation"};
    vk::InstanceCreateInfo instance_ci{};
    instance_ci.pApplicationInfo    = &app_info;
    instance_ci.enabledLayerCount   = debug_layer_name.size();
    instance_ci.ppEnabledLayerNames = &debug_layer_name[0];

    instance = vk::createInstance(instance_ci);
  }

  void create_device() {
    std::vector<vk::PhysicalDevice> physical_devices =
        instance.enumeratePhysicalDevices();

    for (auto pd : physical_devices) {
      vk::PhysicalDeviceProperties pd_property = pd.getProperties();
      if (pd_property.vendorID == 4318 // nvidia
      ) {
        physical_device = pd;
        std::vector<vk::QueueFamilyProperties> pd_queue_family_properties =
            pd.getQueueFamilyProperties();

        std::vector<vk::DeviceQueueCreateInfo> device_queue_ci{};
        device_queue_ci.reserve(2);
        vk::DeviceQueueCreateInfo graphics_queue_ci{};
        vk::DeviceQueueCreateInfo compute_queue_ci{};

        int i = 0;
        for (const auto &qfprops : pd_queue_family_properties) {
          if (!gqueue && qfprops.queueFlags & vk::QueueFlagBits::eGraphics) {
            gq_family_index = i;
            graphics_queue_ci.queueFamilyIndex = i;
            graphics_queue_ci.queueCount       = 1;
            graphics_queue_ci.pQueuePriorities = &gq_priority;
            // graphics_queue_ci.flags = vk::DeviceQueueCreateFlagBits::eProtected;
          }
          if (!cqueue && qfprops.queueFlags & vk::QueueFlagBits::eCompute) {
            cq_family_index = i;
            compute_queue_ci.queueFamilyIndex = i;
            compute_queue_ci.queueCount       = 1;
            compute_queue_ci.pQueuePriorities = &cq_priority;
            // compute_queue_ci.flags = vk::DeviceQueueCreateFlagBits::eProtected;
          }
          i++;
        }
        device_queue_ci.push_back(graphics_queue_ci);
        device_queue_ci.push_back(compute_queue_ci);

        vk::DeviceCreateInfo device_create_info = {};
        device_create_info.pQueueCreateInfos    = &device_queue_ci[0];
        device_create_info.queueCreateInfoCount = device_queue_ci.size();
				device = pd.createDevice(device_create_info);

        gqueue = device.getQueue(gq_family_index, 0);
        cqueue = device.getQueue(cq_family_index, 0);
        return;
      }
    }
    printf("Found no GPU with Vulkan support!"
           " Check your Graphics card and driver.");
  }

  void prepare_pipeline() {
    vk::CommandPoolCreateInfo command_pool_ci{};
    command_pool_ci.queueFamilyIndex = gq_family_index;
    command_pool_ci.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer |
                            vk::CommandPoolCreateFlagBits::eProtected;

    g_command_pool = device.createCommandPool(command_pool_ci);
  }
};

} // namespace CEngine
