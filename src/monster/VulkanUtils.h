#ifndef VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
	#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#endif // !VULKAN_HPP_NO_STRUCT_CONSTRUCTORS


#ifndef VULKAN_UTILS
#define VULKAN_UTILS

#define VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#include <vulkan/vulkan.hpp>
#include "vulkan/vulkan_core.h"

#include <vulkan/vulkan_raii.hpp>



#include <vk_mem_alloc.h>

#include <glm/glm.hpp>

#include <filesystem>

static PFN_vkGetBufferDeviceAddressEXT vkGetBufferDeviceAddressMON;
static PFN_vkWriteResourceDescriptorsEXT vkWriteResourceDescriptorsMON;

typedef  std::vector<vk::raii::DescriptorSet> MonsterDescriptors;
typedef std::vector<vk::Buffer> MonsterBuffers;

struct MBuffer {
	std::vector<vk::raii::Buffer> uboBuffer;
	std::vector<VmaAllocation> bufferAlloc;
	std::vector<VmaAllocationInfo> bufferMapped;
};


struct VulkanTexture {
	vk::Format imgFormat;
	VmaAllocation alloc;
	vk::raii::Image texture = nullptr;
	vk::raii::ImageView textureView = nullptr;
	vk::raii::Sampler textureSampler = nullptr;


};

namespace vulkanUtils {

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

	class Shader {

	public:

		bool compiled = false;
		bool bufferChanged = false;
		bool transformChanged = false;

		//uint32_t shaderIndex;
		std::shared_ptr<vk::raii::Pipeline> graphicsPipeline;

		std::filesystem::path* vertShaderFilePath = nullptr;
		std::filesystem::path* fragShaderFilePath = nullptr;

		// SLANG CODE IS RETAINED UNTIL THE SPV IS NOT COMPILED AND IS LATER CLEARED
		std::vector<char> vertCodeSlang;
		std::vector<char> fragCodeSlang;

		std::vector<uint8_t> vertCodeSpv;
		std::vector<uint8_t> fragCodeSpv;

		// common Buffer containing Tranformation changes
		std::unique_ptr<MBuffer> transformBuffers;
		std::unique_ptr<vk::raii::DescriptorSet> transfromDescriptorSet;

		// Custom Buffers for various shader structures
		std::unique_ptr<MBuffer> uniformBuffers;
		std::unique_ptr<vk::raii::DescriptorSets> descriptorSets;

		virtual void updateBuffers() {};

		void updateTransformBuffer(
			glm::vec3 position,
			glm::vec3 rotation,
			float radiance,
			glm::vec3 scale,
			glm::vec3 camPosition, 
			glm::vec3 camFront, 
			glm::vec3 camUp, 
			float width, 
			float height
		) {
			UniformBufferObject ubo{};

			ubo.model = glm::mat4(1.0f);
			ubo.model = glm::translate(ubo.model, position);
			ubo.model = glm::rotate(ubo.model, glm::radians(radiance), rotation);
			ubo.model = glm::scale(ubo.model, scale);

			ubo.view = glm::lookAt(camPosition, camPosition + camFront, camUp);

			ubo.proj = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 1000.0f);
			
			for (auto tBuffer : transformBuffers->bufferAlloc)
			{
				memcpy(tBuffer, &ubo, sizeof(ubo));
			}
			
		};

		std::vector<std::shared_ptr<VulkanTexture>> textures;

		std::string vertShadername;
		std::string fragShaderName;

		vk::raii::ShaderModule vertexShader = nullptr;
		vk::raii::ShaderModule fragmentShader = nullptr;
		
	};

}

#endif