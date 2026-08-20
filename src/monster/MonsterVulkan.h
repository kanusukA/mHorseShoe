
#ifndef MONSTERVULKAN_H
#define MONSTERVULKAN_H

#include <monster/MonsterSlang.h>

#include <monster/imgui-1.92.9b/imgui.h>
#include <monster/imgui-1.92.9b/backends/imgui_impl_vulkan.h>

#include <iostream>

#include "MonsterSDL.h"

#include <cstddef>
#include <cstdint>
#include <exception>
#include <map>

#include <algorithm>
#include <limits.h>

#if defined (_WIN32)
#define VK_USE_PLATFORM_WIN32_KHR
#endif

//#define VMA_IMPLEMENTATION
////#include <vulkan/vulkan.h>
//#include "vk_mem_alloc.h"


#include <glm/gtc/matrix_transform.hpp>



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


class MonsterVulkan : public MonsterSDL , public MonsterSlang {

public:

	std::vector<std::shared_ptr<hRes::Mesh>> importedMeshes = std::vector<std::shared_ptr<hRes::Mesh>>();
	std::vector<std::shared_ptr<hRes::Mesh>> loadedMeshes = std::vector<std::shared_ptr<hRes::Mesh>>();
	std::vector<std::shared_ptr<hRes::Mesh>> passObjects = std::vector<std::shared_ptr<hRes::Mesh>>();

	std::vector<std::shared_ptr<vk::raii::Pipeline>> pipes = std::vector<std::shared_ptr<vk::raii::Pipeline>>();


	MonsterCamera camera = std::make_unique<Camera>();

	uint32_t windowWidth;
	uint32_t windowHeight;

	VulkanStatus vkMonsterStats = VulkanStatus();
	VulkanMemAlloc vkMemAlloc = VulkanMemAlloc();
	VulkanSync vkSyncStats = VulkanSync();
	VulkanDescriptors vkDescriptors = VulkanDescriptors();

	VulkanTexture* sampleTexture;
	VulkanTexture* depthTexture;

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
	//void createDescriptiorSetLayout();
	void createGraphicsPipeline();
	std::shared_ptr<vk::raii::Pipeline> createGraphicsPipeline(
		const vk::ShaderModule& vertShaderModule,
		const vk::ShaderModule& fragShaderModule,
		const std::string& entryPointVert,
		const std::string& entryPointFrag,
		const vk::DescriptorSetLayout& descriptorLayouts,
		vk::PolygonMode polygonMode = vk::PolygonMode::eFill,
		vk::CullModeFlags cullingModes = vk::CullModeFlagBits::eNone,
		vk::FrontFace frontFace = vk::FrontFace::eClockwise,
		float lineWidth = 1.0f
	);
	void createCommandPool();
	void createCommandBuffer();
	void createDepthResources();
	//void createTextureImage();
	//void createTextureImageView();
	//void createTextureSampler();
	//void createVertexBuffer();
	//uint32_t createVertexBuffer(std::vector<vulkanUtils::Vertex> vertices);
	////void createIndexBuffer();
	//uint32_t createIndexBuffer(std::vector<uint16_t> indices);
	//void createUniformBuffers();
	void createDescriptorPool();
	//void createDescriptorSets();
	void createSyncObjects();


	void updateUniformBuffer(uint32_t currentImage);

	/*void transitionImageLayout(
		vk::raii::CommandBuffer& commandBuffer,
		const vk::raii::Image& image,
		vk::ImageLayout oldLayout,
		vk::ImageLayout newLayout
	);*/

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

	void recordCommandBuffer(uint32_t imageIndex, ImDrawData* drawData);

	void renderVulkanFrame(ImDrawData* drawData);

	void recreateSwapChain();
	void cleanupSwapChain();

	void InitVulkan(uint16_t windowWidth, uint16_t windowHeight);

	void ShutdownVulkan();

	// PIPELINE MODIFERS
	void createSkyBoxPipeline() {};
	void createTerrainPipeline() {};

	// resturns the index of where buffer is stored (in context with the engine Not physical memory)
	void loadMeshToVulkan(std::weak_ptr<hRes::Mesh> mesh);
	void loadMeshShaders(std::weak_ptr<hRes::Mesh> mesh);

	void loadMesh(std::shared_ptr<hRes::Mesh> mesh);

	std::shared_ptr<hRes::Mesh> createNewMesh();
	void loadAllMeshes();
	// CAN BE USED LATER TO SYNC AND ALIGN THE GRAPHICS PIPELINE!!
	// THIS WILL NOT SORT THE RENDER PASS!!
	void loadMeshToPassObject();

	// Shaders must be inside mesh file and must be valid!
	//void createShaderPipeline(uint32_t meshIndex);

	void updateObjectTransforms();
	


};



#endif