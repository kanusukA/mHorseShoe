#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS



#if defined (_WIN32)
	#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include <monster/VulkanUtils.h>

#define VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS
#include <vulkan/vulkan_raii.hpp>

#include <stdlib.h>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>

#include <chrono>



struct VulkanMeshsPipe {
	vk::raii::Pipeline graphicsPipeline = nullptr;
	std::vector<int32_t> meshIndexs = std::vector<int32_t>();
};

struct PassObject {
	int32_t pipelineIndex;
	int32_t vertexIndex;
	int32_t texturesIndex;
};

struct VulkanStatus {

	vk::raii::Context Context;
	vk::raii::Instance vkInstance = nullptr;
	vk::raii::Device device = nullptr;
	vk::raii::PhysicalDevice gpuDevice = nullptr;   
	vk::raii::Queue graphicsQueue = nullptr;
	vk::raii::SurfaceKHR surface = nullptr;
	vk::raii::SwapchainKHR swapChain = nullptr;
	
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

	vk::raii::Pipeline graphicsPipeline = nullptr;
	vk::raii::Pipeline imguiPipeline = nullptr;

	vk::raii::PipelineLayout imguiPipeLayout = nullptr;
	
};

struct VulkanSync {

	std::vector<vk::raii::Semaphore> presentCompleteSemaphores;
	std::vector<vk::raii::Semaphore> renderFinishedSemaphores;
	std::vector<vk::raii::Fence> inFlightFences;

};





struct VulkanMemAlloc {

	VmaAllocator vmaAllocator;

	std::vector<vk::raii::Buffer> vertexBuffer;
	std::vector<VmaAllocation> vertexBufferAlloc;
	std::vector<vk::raii::Buffer> indexBuffer;
	std::vector<VmaAllocation> indexBufferAlloc;

	std::vector<uint32_t> indexes;
	std::vector<uint32_t> vertices;

	std::vector<std::shared_ptr<MBuffer>> uniformBuffers;

};

struct VulkanDescriptors {
	vk::raii::DescriptorPool descriptorPool = nullptr;
	vk::raii::PipelineLayout pipelineLayout = nullptr;
	vk::raii::DescriptorSetLayout descriptorSetLayout = nullptr;
	
};






