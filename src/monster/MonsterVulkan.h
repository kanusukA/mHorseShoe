
#include <monster/VulkanStats.h>

#include <monster/imgui-1.92.9b/imgui.h>

#include <GDHandler/ResourceHandler.h>

#include <stdio.h>
#include <iostream>

#include "MonsterSDL.h"

#include <Camera.h>
// VALIDATION LAYERS
const std::vector<char const*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};


static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(
	vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
	vk::DebugUtilsMessageTypeFlagsEXT type,
	const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData
) {
	std::cerr << "validation layer: type " << vk::to_string(type) << " msg: " << pCallbackData->pMessage << std::endl;

	return vk::False;
}

constexpr bool enableValidationLayers = true;

typedef std::unique_ptr<Camera> MonsterCamera;


class MonsterVulkan : public MonsterSDL {

public:

	MonsterCamera camera = std::make_unique<Camera>();

	uint32_t windowWidth;
	uint32_t windowHeight;

	VulkanStatus vkMonsterStats = VulkanStatus();
	VulkanMemAlloc vkMemAlloc = VulkanMemAlloc();
	VulkanSync vkSyncStats = VulkanSync();
	VulkanTextures vkTextures = VulkanTextures();
	VulkanDescriptors vkDescriptors = VulkanDescriptors();

	vk::raii::DebugUtilsMessengerEXT debugMessenger = nullptr;

	void createVulkanInstance();
	void setupDebugMessenger();
	void createVulkanSurface();
	void pickVulkanPhysicalDevice();
	void createVulkanDevice();
	void createVulkanMemAllocator();
	void createSwapchain();
	void createImageView();
	vk::raii::ImageView createImageView(vk::Image const& image, vk::Format format, vk::ImageAspectFlags flags);
	void createDescriptiorSetLayout();
	void createGraphicsPipeline();
	void createCommandPool();
	void createCommandBuffer();
	void createDepthResources();
	void createTextureImage();
	void createTextureImageView();
	void createTextureSampler();
	void createVertexBuffer();
	void createVertexBuffer(std::vector<horse::Vertex> vertices);
	void createIndexBuffer();
	void createIndexBuffer(std::vector<uint16_t> indices);
	void createUniformBuffers();
	void createDescriptorPool();
	void createDescriptorSets();
	void createSyncObjects();

	std::pair<VkBuffer, VmaAllocation> createBuffer(
		vk::DeviceSize size,
		vk::BufferUsageFlags usage,
		VmaAllocationCreateFlags allocFlags,
		VmaMemoryUsage allocUsage,
		VmaAllocationInfo* allocationInfo = nullptr
	);
	std::pair<vk::raii::Image, VmaAllocation> createImage(
		uint32_t width,
		uint32_t height,
		vk::Format format,
		vk::ImageTiling tiling,
		vk::ImageUsageFlags usage,
		VmaAllocationCreateFlags allocFlags,
		VmaMemoryUsage allocUsage
	);

	void copyBuffer(
		vk::Buffer srcBuffer,
		vk::Buffer dstBuffer,
		vk::DeviceSize bufferSize
	);

	void copyBufferToImage(
		vk::raii::CommandBuffer& commandBuffer,
		VkBuffer buffer,
		vk::raii::Image& image,
		uint32_t width,
		uint32_t height
	);

	void updateUniformBuffer(uint32_t currentImage);

	void transitionImageLayout(
		vk::raii::CommandBuffer& commandBuffer,
		const vk::raii::Image& image,
		vk::ImageLayout oldLayout,
		vk::ImageLayout newLayout
	);

	void transition_image_layout(
		uint32_t imageIndex,
		vk::ImageLayout old_layout,
		vk::ImageLayout new_layout,
		vk::AccessFlags2 src_access_mask,
		vk::AccessFlags2 dst_access_mask,
		vk::PipelineStageFlags2 src_stage_mask,
		vk::PipelineStageFlags2 dst_stage_mask,
		vk::ImageAspectFlags flags
	);

	vk::Format findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling, vk::FormatFeatureFlags features);
	vk::Format findDepthFormat();

	vk::raii::ShaderModule createShaderModule(const std::vector<char>& code) const;
	vk::raii::ShaderModule createShaderModule(const uint32_t* code, size_t codeSize) const;

	vk::raii::CommandBuffer begineSingleTimeCommands();
	void endSingleTimeCommands(vk::raii::CommandBuffer&& commandBuffer);

	void recordCommandBuffer(uint32_t imageIndex, ImDrawData* drawData);

	void renderVulkanFrame(ImDrawData* drawData);

	void recreateSwapChain();
	void cleanupSwapChain();

	void InitVulkan(uint16_t windowWidth, uint16_t windowHeight);


	void ShutdownVulkan();

	//

};



