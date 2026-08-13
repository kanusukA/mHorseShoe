#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS

// VULKAN
constexpr int MAX_FRAMES_IN_FLIGHT = 2;

#if defined (_WIN32)
	#define VK_USE_PLATFORM_WIN32_KHR
#endif




#include <monster/VulkanUtils.h>

#define VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS
#include <vulkan/vulkan_raii.hpp>

#include <vk_mem_alloc.h>

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

	std::vector<vk::raii::Buffer> uniformBuffers;
	std::vector<VmaAllocation> uniformBufferAlloc;
	std::vector<void*> uniformBuffersMapped;

};

struct VulkanDescriptors {
	vk::raii::DescriptorPool descriptorPool = nullptr;
	vk::raii::PipelineLayout pipelineLayout = nullptr;
	vk::raii::DescriptorSetLayout descriptorSetLayout = nullptr;

	std::vector<vk::raii::DescriptorSet> descriptorSets;
	
};

struct VulkanTextures {

	// DEPTH
	vk::raii::Image depthImage = nullptr;
	vk::raii::ImageView depthImageView = nullptr;
	vk::Format depthFormat;
	VmaAllocation depthImageAlloc;

	// Normal Texture
	vk::raii::Image textureImage = nullptr;
	vk::raii::ImageView textureImageView = nullptr;
	vk::raii::Sampler textureSampler = nullptr;
	VmaAllocation textureAlloc;
};

struct UniformBufferObject {
	/*glm::vec2 foo;
	alignas(16)*/ // YOU CAN ALSO USE GLM_FORCE_DEFAULT_ALIGNED_GENTYPES for consistent alignment but it does not work in nested struct
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};


