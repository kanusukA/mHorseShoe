
#include <monster/MonsterSlang.h>

#include <monster/imgui-1.92.9b/imgui.h>
#include <monster/imgui-1.92.9b/backends/imgui_impl_vulkan.h>

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


class MonsterVulkan :public MonsterSlang , public MonsterSDL {

public:

	std::vector<std::shared_ptr<hRes::Mesh>> importedMeshes = std::vector<std::shared_ptr<hRes::Mesh>>();
	std::vector<uint32_t> loadedMeshes = std::vector<uint32_t>();
	std::vector<uint32_t> passObjects = std::vector<uint32_t>();

	std::vector<vk::raii::Pipeline> pipes = std::vector<vk::raii::Pipeline>();

	MonsterCamera camera = std::make_unique<Camera>();

	uint32_t windowWidth;
	uint32_t windowHeight;

	VulkanStatus vkMonsterStats = VulkanStatus();
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
	void createDescriptorSetLayout();
	uint32_t createDescriptorSetLayout(const std::vector<vk::DescriptorSetLayoutBinding>& bindings);
	void createDescriptorSetLayout(const std::vector<vk::DescriptorSetLayoutBinding>& bindings, vk::raii::DescriptorSetLayout* descriptorLayout);
	void createGraphicsPipeline();
	vk::raii::Pipeline createGraphicsPipeline(
		const vk::ShaderModule& vertShaderModule,
		const vk::ShaderModule& fragShaderModule,
		uint32_t pipelineLayoutIndex,
		vk::PolygonMode polygonMode = vk::PolygonMode::eFill,
		vk::CullModeFlags cullingModes = vk::CullModeFlagBits::eNone,
		vk::FrontFace frontFace = vk::FrontFace::eCounterClockwise,
		float lineWidth = 1.0f
	);
	std::pair<vk::raii::Pipeline,vk::raii::PipelineLayout> createGraphicsPipeline(
		const vk::ShaderModule& vertShaderModule,
		const vk::ShaderModule& fragShaderModule,
		const vk::raii::DescriptorSetLayout& setLayout,
		const uint32_t& pushConstantSize = uint32_t(0),
		vk::PolygonMode polygonMode = vk::PolygonMode::eFill,
		vk::CullModeFlags cullingModes = vk::CullModeFlagBits::eNone,
		vk::FrontFace frontFace = vk::FrontFace::eCounterClockwise,
		bool colorBlending = false,
		float lineWidth = 1.0f
	);
	void createCommandPool();
	void createCommandBuffer();
	void createDepthResources();
	void createTextureImage();
	void createTexture(const std::filesystem::path& path, MonsterTexture* texture);
	void createTextureImage(const std::filesystem::path& path, vk::raii::Image* image);
	void createTextureImageView();
	void createTextureImageView(const vk::raii::Image& image, vk::raii::ImageView* imageView);
	void createTextureSampler();
	void createTextureSampler(vk::raii::Sampler* sampler);
	void createDescriptorPool();
	void createDescriptorSets();
	void createDescriptorSets(const vk::raii::DescriptorSetLayout& setLayout,std::vector<vk::raii::DescriptorSets>* descriptorSets);
	uint32_t createDescriptorSets(
		uint32_t descriptorSetLayout
	);
	void updateDescriptorSets(const std::vector<std::vector<vk::WriteDescriptorSet>>& descriptorWrites);
	void createSyncObjects();

	std::pair<vk::raii::Image, VmaAllocation> createImage(
		uint32_t width,
		uint32_t height,
		vk::Format format,
		vk::ImageTiling tiling,
		vk::ImageUsageFlags usage,
		VmaAllocationCreateFlags allocFlags,
		VmaMemoryUsage allocUsage
	);

	void copyBufferToImage(
		vk::raii::CommandBuffer& commandBuffer,
		VkBuffer buffer,
		vk::raii::Image& image,
		uint32_t width,
		uint32_t height
	);

	void updateUniformBuffer(uint32_t currentImage,uint32_t uboIndex);

	void updateUniformBuffer(uint32_t currentImage, void* bufferMapped);

	void transitionImageLayout(
		vk::raii::CommandBuffer& commandBuffer,
		const vk::raii::Image& image,
		vk::ImageLayout oldLayout,
		vk::ImageLayout newLayout
	);

	void transition_image_layout(
		vk::Image image,
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

	/*vk::raii::ShaderModule createShaderModule(const std::vector<char>& code) const;
	vk::raii::ShaderModule createShaderModule(const uint32_t* code, size_t codeSize) const;*/

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
	void loadMeshToVulkan(uint32_t meshIndex);
	void loadMeshShaders(uint32_t shaderIndex,uint32_t meshIndex);
	void loadMeshShader(uint32_t meshIndex);

	void loadMesh(uint32_t shaderIndex, uint32_t meshIndex);
	void loadMeshContainingShader(uint32_t meshIndex);

	void importMesh(hRes::Mesh& mesh);
	
	std::weak_ptr<hRes::Mesh> createMesh();

	void addMesh(std::shared_ptr<hRes::Mesh> mesh);

	void loadAllMeshes();
	// CAN BE USED LATER TO SYNC AND ALIGN THE GRAPHICS PIPELINE!!
	// THIS WILL NOT SORT THE RENDER PASS!!
	void loadMeshToPassObject();

	// Shaders must be inside mesh file and must be valid!
	//void createShaderPipeline(uint32_t meshIndex);

};



