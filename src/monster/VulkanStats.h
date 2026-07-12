#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#define VK_USE_PLATFORM_WIN32_KHR

#include <vulkan/vulkan_raii.hpp>



struct VulkanStatus {

	vk::raii::Context Context;
	vk::raii::Instance vkInstance = nullptr;
	vk::raii::Device device = nullptr;
	vk::raii::PhysicalDevice gpuDevice = nullptr;   
	vk::raii::Queue graphicsQueue = nullptr;
	vk::raii::SurfaceKHR surface = nullptr;

	bool vulkanInitalized = false;

	bool supportsVulkan1_3 = false;
	bool supportsGraphic = false;
	bool supportsRequiredExtensions = false;
	bool supportsRequiredFeatures= false;
	


};