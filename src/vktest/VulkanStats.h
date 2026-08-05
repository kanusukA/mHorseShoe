#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS

// VULKAN
constexpr int MAX_FRAMES_IN_FLIGHT = 2;

#if defined (_WIN32)
	#define VK_USE_PLATFORM_WIN32_KHR
#endif



#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_core.h"

#define VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS
#include <vulkan/vulkan_raii.hpp>

#include <imgui.h>

#include <vk_mem_alloc.h>

#include <SDL3/SDL.h>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>


#include <chrono>


// IMGUI SHADER CODE
// VERTEX SHADER
static uint32_t __glsl_shader_vert_spv[] =
{
	0x07230203,0x00010000,0x0008000b,0x0000002e,0x00000000,0x00020011,0x00000001,0x0006000b,
	0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
	0x000a000f,0x00000000,0x00000004,0x6e69616d,0x00000000,0x0000000b,0x0000000f,0x00000015,
	0x0000001b,0x0000001c,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,
	0x00000000,0x00030005,0x00000009,0x00000000,0x00050006,0x00000009,0x00000000,0x6f6c6f43,
	0x00000072,0x00040006,0x00000009,0x00000001,0x00005655,0x00030005,0x0000000b,0x0074754f,
	0x00040005,0x0000000f,0x6c6f4361,0x0000726f,0x00030005,0x00000015,0x00565561,0x00060005,
	0x00000019,0x505f6c67,0x65567265,0x78657472,0x00000000,0x00060006,0x00000019,0x00000000,
	0x505f6c67,0x7469736f,0x006e6f69,0x00030005,0x0000001b,0x00000000,0x00040005,0x0000001c,
	0x736f5061,0x00000000,0x00060005,0x0000001e,0x73755075,0x6e6f4368,0x6e617473,0x00000074,
	0x00050006,0x0000001e,0x00000000,0x61635375,0x0000656c,0x00060006,0x0000001e,0x00000001,
	0x61725475,0x616c736e,0x00006574,0x00030005,0x00000020,0x00006370,0x00040047,0x0000000b,
	0x0000001e,0x00000000,0x00040047,0x0000000f,0x0000001e,0x00000002,0x00040047,0x00000015,
	0x0000001e,0x00000001,0x00030047,0x00000019,0x00000002,0x00050048,0x00000019,0x00000000,
	0x0000000b,0x00000000,0x00040047,0x0000001c,0x0000001e,0x00000000,0x00030047,0x0000001e,
	0x00000002,0x00050048,0x0000001e,0x00000000,0x00000023,0x00000000,0x00050048,0x0000001e,
	0x00000001,0x00000023,0x00000008,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,
	0x00030016,0x00000006,0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040017,
	0x00000008,0x00000006,0x00000002,0x0004001e,0x00000009,0x00000007,0x00000008,0x00040020,
	0x0000000a,0x00000003,0x00000009,0x0004003b,0x0000000a,0x0000000b,0x00000003,0x00040015,
	0x0000000c,0x00000020,0x00000001,0x0004002b,0x0000000c,0x0000000d,0x00000000,0x00040020,
	0x0000000e,0x00000001,0x00000007,0x0004003b,0x0000000e,0x0000000f,0x00000001,0x00040020,
	0x00000011,0x00000003,0x00000007,0x0004002b,0x0000000c,0x00000013,0x00000001,0x00040020,
	0x00000014,0x00000001,0x00000008,0x0004003b,0x00000014,0x00000015,0x00000001,0x00040020,
	0x00000017,0x00000003,0x00000008,0x0003001e,0x00000019,0x00000007,0x00040020,0x0000001a,
	0x00000003,0x00000019,0x0004003b,0x0000001a,0x0000001b,0x00000003,0x0004003b,0x00000014,
	0x0000001c,0x00000001,0x0004001e,0x0000001e,0x00000008,0x00000008,0x00040020,0x0000001f,
	0x00000009,0x0000001e,0x0004003b,0x0000001f,0x00000020,0x00000009,0x00040020,0x00000021,
	0x00000009,0x00000008,0x0004002b,0x00000006,0x00000028,0x00000000,0x0004002b,0x00000006,
	0x00000029,0x3f800000,0x00050036,0x00000002,0x00000004,0x00000000,0x00000003,0x000200f8,
	0x00000005,0x0004003d,0x00000007,0x00000010,0x0000000f,0x00050041,0x00000011,0x00000012,
	0x0000000b,0x0000000d,0x0003003e,0x00000012,0x00000010,0x0004003d,0x00000008,0x00000016,
	0x00000015,0x00050041,0x00000017,0x00000018,0x0000000b,0x00000013,0x0003003e,0x00000018,
	0x00000016,0x0004003d,0x00000008,0x0000001d,0x0000001c,0x00050041,0x00000021,0x00000022,
	0x00000020,0x0000000d,0x0004003d,0x00000008,0x00000023,0x00000022,0x00050085,0x00000008,
	0x00000024,0x0000001d,0x00000023,0x00050041,0x00000021,0x00000025,0x00000020,0x00000013,
	0x0004003d,0x00000008,0x00000026,0x00000025,0x00050081,0x00000008,0x00000027,0x00000024,
	0x00000026,0x00050051,0x00000006,0x0000002a,0x00000027,0x00000000,0x00050051,0x00000006,
	0x0000002b,0x00000027,0x00000001,0x00070050,0x00000007,0x0000002c,0x0000002a,0x0000002b,
	0x00000028,0x00000029,0x00050041,0x00000011,0x0000002d,0x0000001b,0x0000000d,0x0003003e,
	0x0000002d,0x0000002c,0x000100fd,0x00010038
};

static uint32_t __glsl_shader_frag_spv[] =
{
	0x07230203,0x00010000,0x0008000b,0x00000023,0x00000000,0x00020011,0x00000001,0x0006000b,
	0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
	0x0007000f,0x00000004,0x00000004,0x6e69616d,0x00000000,0x00000009,0x0000000d,0x00030010,
	0x00000004,0x00000007,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,
	0x00000000,0x00040005,0x00000009,0x6c6f4366,0x0000726f,0x00030005,0x0000000b,0x00000000,
	0x00050006,0x0000000b,0x00000000,0x6f6c6f43,0x00000072,0x00040006,0x0000000b,0x00000001,
	0x00005655,0x00030005,0x0000000d,0x00006e49,0x00050005,0x00000015,0x7865545f,0x65727574,
	0x00000000,0x00050005,0x00000019,0x6d61535f,0x72656c70,0x00000000,0x00040047,0x00000009,
	0x0000001e,0x00000000,0x00040047,0x0000000d,0x0000001e,0x00000000,0x00040047,0x00000015,
	0x00000021,0x00000000,0x00040047,0x00000015,0x00000022,0x00000000,0x00040047,0x00000019,
	0x00000021,0x00000000,0x00040047,0x00000019,0x00000022,0x00000001,0x00020013,0x00000002,
	0x00030021,0x00000003,0x00000002,0x00030016,0x00000006,0x00000020,0x00040017,0x00000007,
	0x00000006,0x00000004,0x00040020,0x00000008,0x00000003,0x00000007,0x0004003b,0x00000008,
	0x00000009,0x00000003,0x00040017,0x0000000a,0x00000006,0x00000002,0x0004001e,0x0000000b,
	0x00000007,0x0000000a,0x00040020,0x0000000c,0x00000001,0x0000000b,0x0004003b,0x0000000c,
	0x0000000d,0x00000001,0x00040015,0x0000000e,0x00000020,0x00000001,0x0004002b,0x0000000e,
	0x0000000f,0x00000000,0x00040020,0x00000010,0x00000001,0x00000007,0x00090019,0x00000013,
	0x00000006,0x00000001,0x00000000,0x00000000,0x00000000,0x00000001,0x00000000,0x00040020,
	0x00000014,0x00000000,0x00000013,0x0004003b,0x00000014,0x00000015,0x00000000,0x0002001a,
	0x00000017,0x00040020,0x00000018,0x00000000,0x00000017,0x0004003b,0x00000018,0x00000019,
	0x00000000,0x0003001b,0x0000001b,0x00000013,0x0004002b,0x0000000e,0x0000001d,0x00000001,
	0x00040020,0x0000001e,0x00000001,0x0000000a,0x00050036,0x00000002,0x00000004,0x00000000,
	0x00000003,0x000200f8,0x00000005,0x00050041,0x00000010,0x00000011,0x0000000d,0x0000000f,
	0x0004003d,0x00000007,0x00000012,0x00000011,0x0004003d,0x00000013,0x00000016,0x00000015,
	0x0004003d,0x00000017,0x0000001a,0x00000019,0x00050056,0x0000001b,0x0000001c,0x00000016,
	0x0000001a,0x00050041,0x0000001e,0x0000001f,0x0000000d,0x0000001d,0x0004003d,0x0000000a,
	0x00000020,0x0000001f,0x00050057,0x00000007,0x00000021,0x0000001c,0x00000020,0x00050085,
	0x00000007,0x00000022,0x00000012,0x00000021,0x0003003e,0x00000009,0x00000022,0x000100fd,
	0x00010038
};




class ImGuiVulkanUtil {
protected:
	vk::raii::Sampler sampler{ nullptr };
	vk::Buffer vertexBuffers;
	vk::Buffer indexBuffers;
	VmaAllocation vertexAllocation;
	VmaAllocation indexAllocation;
	uint32_t vertexCount = 0;
	uint32_t indexCount = 0;
	vk::raii::Image fontImage = nullptr;
	vk::raii::ImageView fontImageView = nullptr;

	vk::raii::PipelineCache pipelineCache{ nullptr };
	vk::raii::PipelineLayout pipelineLayout{ nullptr };
	vk::raii::Pipeline pipeline{ nullptr };
	vk::raii::DescriptorPool descriptorPool{ nullptr };
	vk::raii::DescriptorSetLayout descriptorSetLayout{ nullptr };
	vk::raii::DescriptorSet descriptorSet{ nullptr };

	vk::raii::Device* device = nullptr;
	vk::raii::PhysicalDevice* physicalDevice = nullptr;
	vk::raii::Queue* graphicsQueue = nullptr;
	uint32_t graphicsQueueFamily = 0;

	VmaAllocator allocator = nullptr;

	vk::raii::CommandBuffer commandBuffer = nullptr;


	ImGuiStyle vulkanStyle;

	vk::raii::ShaderModule imguiVertShader = nullptr;
	vk::raii::ShaderModule imguiFragShader = nullptr;



	bool needsUpdateBuffers = false;

	vk::PipelineRenderingCreateInfo renderingInfo{};
	vk::Format colorFormat = vk::Format::eB8G8R8A8Unorm;


public:

	struct PushConstBlock {
		glm::vec2 scale;
		glm::vec2 translate;
	}pushConstBlock;

	ImGuiVulkanUtil() {};
	~ImGuiVulkanUtil();

	virtual void setUtils(vk::raii::Device& p_device, vk::raii::PhysicalDevice& p_physicalDevice,
		vk::raii::Queue& graphicsQueue, uint32_t graphicsQueueFamily, VmaAllocator allocator) {};

	void init(float width, float height);
	void initResources();
	void setStyle(uint32_t index);
	virtual void updateTexture(vk::raii::CommandBuffer& commandBuffer, ImTextureData* tex) {};
	virtual void createImGuiShaders() {};

	virtual void createImGuiPipeline() {};

	bool newFrame();
	virtual void updateBuffers(uint32_t frameIndex) {};
	void drawFrame(vk::raii::CommandBuffer& commandBuffer);

	void handleKey(SDL_Event& event) {};



};

struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;

	static vk::VertexInputBindingDescription getBindingDescription() {
		return { .binding = 0, .stride = sizeof(Vertex), .inputRate = vk::VertexInputRate::eVertex };
	}

	static std::array<vk::VertexInputAttributeDescription, 3> getAttributeDescriptions() {
		return { {
			{.location = 0, .binding = 0, .format = vk::Format::eR32G32B32Sfloat, .offset = offsetof(Vertex, pos)},
			{.location = 1, .binding = 0, .format = vk::Format::eR32G32B32Sfloat, .offset = offsetof(Vertex,color)},
			{.location = 2, .binding = 0, .format = vk::Format::eR32G32Sfloat, .offset = offsetof(Vertex,texCoord)},
			} };
	}
};


struct Mesh {
	std::vector<Vertex>* vertices = nullptr;
	std::vector<uint16_t>* indices = nullptr;

	glm::vec4 color = glm::vec4(1, 1, 1, 1);

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

struct VulkanMemAlloc {
	VmaAllocator vmaAllocator;

	vk::raii::Buffer vertexBuffer = nullptr;
	VmaAllocation vertexBufferAlloc;
	vk::raii::Buffer indexBuffer = nullptr;
	VmaAllocation indexBufferAlloc;

	uint32_t indexes;

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
