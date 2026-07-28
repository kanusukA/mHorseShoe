#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS



#if defined (_WIN32)
	#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_core.h"

#define VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS
#include <vulkan/vulkan_raii.hpp>


#include <glm/glm.hpp>

struct Vertex {
	glm::vec2 pos;
	glm::vec3 color;

	static vk::VertexInputBindingDescription getBindingDescription() {
		return { .binding = 0, .stride = sizeof(Vertex), .inputRate = vk::VertexInputRate::eVertex };
	}

	static std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions() {
		return { {
			{.location = 0, .binding = 0, .format = vk::Format::eR32G32Sfloat, .offset = offsetof(Vertex, pos)},
			{.location = 1, .binding = 0, .format = vk::Format::eR32G32B32Sfloat, .offset = offsetof(Vertex,color)}
			} };
	}
};



struct VulkanStatus {

	vk::raii::Context Context;
	vk::raii::Instance vkInstance = nullptr;
	vk::raii::Device device = nullptr;
	vk::raii::PhysicalDevice gpuDevice = nullptr;   
	vk::raii::Queue graphicsQueue = nullptr;
	vk::raii::SurfaceKHR surface = nullptr;
	vk::raii::SwapchainKHR swapChain = nullptr;
	vk::raii::Pipeline graphicsPipeline = nullptr;
	vk::SurfaceFormatKHR swapChainSurfaceFormat;
	vk::Extent2D swapChainExtent;
	
	std::vector<vk::Image> swapChainImages;
	std::vector<vk::raii::ImageView> swapChainImageViews;

	uint32_t queueIndex;

	uint32_t frameIndex = 0;

	vk::raii::CommandPool commandPool = nullptr;
	std::vector<vk::raii::CommandBuffer> commandBuffers;

	bool vulkanInitalized = false;

	bool supportsVulkan1_3 = false;
	bool supportsGraphic = false;
	bool supportsRequiredExtensions = false;
	bool supportsRequiredFeatures= false;
	


};

struct VulkanSync {
	std::vector<vk::raii::Semaphore> presentCompleteSemaphores;
	std::vector<vk::raii::Semaphore> renderFinishedSemaphores;
	std::vector<vk::raii::Fence> inFlightFences;
};
