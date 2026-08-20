#ifndef VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
	#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#endif // !VULKAN_HPP_NO_STRUCT_CONSTRUCTORS





#ifndef VULKAN_UTILS
#define VULKAN_UTILS

constexpr int MAX_FRAMES_IN_FLIGHT = 2;

constexpr int MAX_OBJECT = 20;

#define VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#include <vulkan/vulkan.hpp>
#include "vulkan/vulkan_core.h"

#include <vulkan/vulkan_raii.hpp>

#include <vk_mem_alloc.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <filesystem>

// BUFFERS FOR SHADERS
struct UniformBufferObject {
	/*glm::vec2 foo;
	alignas(16)*/ // YOU CAN ALSO USE GLM_FORCE_DEFAULT_ALIGNED_GENTYPES for consistent alignment but it does not work in nested struct
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

// UNIFORM BUFFERS
// SKYBOX
struct SkyUniformBuffer {

	float highlightOffset;
	float highlightSmoothness;
	float midOffset;
	float midSmoothness;
	float coreOffset;
	float coreSmoothness;
	float bumpOffset;
	float bumpRange;
	float bumpHeight;
	glm::vec4 baseCol;
	glm::vec4 highCol;
	glm::vec4 midCol;
	glm::vec4 coreCol;

};


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
		std::unique_ptr<MBuffer> buffers;
		std::unique_ptr<vk::raii::DescriptorSets> descriptorSets;
		std::unique_ptr<vk::raii::DescriptorSetLayout> descriptorSetLayout;

		// Custom Buffers for various shader structures
		//std::unique_ptr<MBuffer> uniformBuffers;
		// The outer vector contains each descriptor for each frame in flight
		//std::unique_ptr<vk::raii::DescriptorSets> descriptorSets;
		std::unique_ptr<MBuffer> CustomBuffers;

		std::vector<std::shared_ptr<VulkanTexture>> textures;

		std::string vertShadername;
		std::string fragShaderName;

		vk::raii::ShaderModule vertexShader = nullptr;
		vk::raii::ShaderModule fragmentShader = nullptr;

		virtual void updateBuffers() {
			
		};

		void updateTransformBuffer(
			glm::vec3 position,
			glm::vec3 rotation,
			float radiance,
			glm::vec3 scale,
			glm::vec3& camPosition, 
			glm::vec3& camFront, 
			glm::vec3& camUp, 
			float width, 
			float height
			
		) {
			UniformBufferObject ubo{};

			ubo.model = glm::mat4(1.0f);
			/*ubo.model = glm::translate(ubo.model, position);
			ubo.model = glm::rotate(ubo.model, glm::radians(radiance), rotation);
			ubo.model = glm::scale(ubo.model, scale);*/
			ubo.model = glm::rotate(glm::mat4(1.0f),  glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

			ubo.view = glm::lookAt(glm::vec3(2.0f,2.0f,2.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

			ubo.proj = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 1000.0f);

			ubo.proj[1][1] *= -1;
			
			for (auto tBuffer : buffers->bufferMapped)
			{
				memcpy(tBuffer.pMappedData, &ubo, sizeof(ubo));
			}
			
		};

		virtual vk::DescriptorSetLayoutCreateInfo& createDescriptorSetLayouts(vk::raii::Device* device, const vk::DescriptorPool* pool) {
			// set layout
			std::array<vk::DescriptorSetLayoutBinding, 2> bindings{
				{{
				.binding = 0,
				.descriptorType = vk::DescriptorType::eUniformBuffer,
				.descriptorCount = 1,
				.stageFlags = vk::ShaderStageFlagBits::eVertex
				},{
				.binding = 1,
				.descriptorType = vk::DescriptorType::eCombinedImageSampler,
				.descriptorCount = 1,
				.stageFlags = vk::ShaderStageFlagBits::eFragment
				}}
			};

			vk::DescriptorSetLayoutCreateInfo layoutInfo{
				.bindingCount = static_cast<uint32_t>(bindings.size()),
				.pBindings = bindings.data()
			};


			descriptorSetLayout = std::make_unique<vk::raii::DescriptorSetLayout>(vk::raii::DescriptorSetLayout(*device, layoutInfo));

			std::vector<vk::DescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, *descriptorSetLayout.get());
			
			vk::DescriptorSetAllocateInfo allocInfo{
				.descriptorPool = *pool,
				.descriptorSetCount = static_cast<uint32_t>(layouts.size()),
				.pSetLayouts = layouts.data()
			};

			descriptorSets = std::make_unique<vk::raii::DescriptorSets>(device->allocateDescriptorSets(allocInfo));

			return layoutInfo;

		}

		virtual void createDescriptorWriteInfo(vk::raii::Device* device) {

			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			{

				std::vector<vk::WriteDescriptorSet> descriptorWriteSet = std::vector<vk::WriteDescriptorSet>();

				vk::DescriptorBufferInfo uboInfo{
					.buffer = buffers->uboBuffer[i],
					.offset = 0,
					.range = sizeof(UniformBufferObject)
				};

				vk::WriteDescriptorSet writeSet{
					.dstSet = descriptorSets->at(i),
					.dstBinding = 0,
					.dstArrayElement = 0,
					.descriptorCount = 1,
					.descriptorType = vk::DescriptorType::eUniformBuffer,
					.pBufferInfo = &uboInfo
				};

				descriptorWriteSet.push_back(writeSet);

				
				vk::DescriptorImageInfo imageInfo{
					.sampler = textures.at(0)->textureSampler,
					.imageView = textures.at(0)->textureView,
					.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal
				};

				vk::WriteDescriptorSet writeImageSet{
				.dstSet = descriptorSets->at(i),
				.dstBinding = 1,
				.dstArrayElement = 0,
				.descriptorCount = 1,
				.descriptorType = vk::DescriptorType::eCombinedImageSampler,
				.pBufferInfo = &uboInfo
				};

				descriptorWriteSet.push_back(writeImageSet);
				

				device->updateDescriptorSets(descriptorWriteSet, {});

			}
		}	
	};

	class SkyShader : public Shader {
	public:

		std::unique_ptr<SkyUniformBuffer> skyUniformBuffer = std::make_unique<SkyUniformBuffer>();



		virtual vk::DescriptorSetLayoutCreateInfo& createDescriptorSetLayouts(vk::raii::Device* device, const vk::DescriptorPool* pool) override {

			std::array<vk::DescriptorSetLayoutBinding, 3> bindings{
				{{
				.binding = 0,
				.descriptorType = vk::DescriptorType::eUniformBuffer,
				.descriptorCount = 1,
				.stageFlags = vk::ShaderStageFlagBits::eVertex
				},{
				.binding = 2,
				.descriptorType = vk::DescriptorType::eUniformBuffer,
				.descriptorCount = 1,
				.stageFlags = vk::ShaderStageFlagBits::eFragment
				},{
				.binding = 1,
				.descriptorType = vk::DescriptorType::eCombinedImageSampler,
				.descriptorCount = 1,
				.stageFlags = vk::ShaderStageFlagBits::eFragment
				}}
			};

			vk::DescriptorSetLayoutCreateInfo layoutInfo{
				.bindingCount = static_cast<uint32_t>(bindings.size()),
				.pBindings = bindings.data()
			};

			
			descriptorSetLayout = std::make_unique<vk::raii::DescriptorSetLayout>(vk::raii::DescriptorSetLayout(*device, layoutInfo));

			std::vector<vk::DescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, *descriptorSetLayout.get());

			vk::DescriptorSetAllocateInfo allocInfo{
				.descriptorPool = *pool,
				.descriptorSetCount = static_cast<uint32_t>(layouts.size()),
				.pSetLayouts = layouts.data()
			};

			descriptorSets = std::make_unique<vk::raii::DescriptorSets>(device->allocateDescriptorSets(allocInfo));

			return layoutInfo;
		}

		virtual void createDescriptorWriteInfo(vk::raii::Device* device) override {
			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			{

				std::vector<vk::WriteDescriptorSet> descriptorWriteSet = std::vector<vk::WriteDescriptorSet>();

				vk::DescriptorBufferInfo uboInfo{
					.buffer = buffers->uboBuffer.at(i),
					.offset = 0,
					.range = sizeof(UniformBufferObject)
				};

				vk::WriteDescriptorSet writeSet{
					.dstSet = descriptorSets->at(i),
					.dstBinding = 0,
					.dstArrayElement = 0,
					.descriptorType = vk::DescriptorType::eUniformBuffer,
					.pBufferInfo = &uboInfo
				};

				descriptorWriteSet.push_back(writeSet);

				if (textures.size() > 0)
				{
					vk::DescriptorImageInfo imageInfo{
						.sampler = textures.at(0)->textureSampler,
						.imageView = textures.at(0)->textureView,
						.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal
					};

					vk::WriteDescriptorSet writeImageSet{
					.dstSet = descriptorSets->at(i),
					.dstBinding = 1,
					.dstArrayElement = 0,
					.descriptorType = vk::DescriptorType::eCombinedImageSampler,
					.pBufferInfo = &uboInfo
					};

					descriptorWriteSet.push_back(writeImageSet);
				}

				vk::DescriptorBufferInfo skyBufferInfo{
					.buffer = CustomBuffers->uboBuffer.at(i),
					.offset = 0,
					.range = sizeof(SkyUniformBuffer)
				};

				vk::WriteDescriptorSet skyBufferSet{
					.dstSet = descriptorSets->at(i),
					.dstBinding = 2,
					.dstArrayElement = 0,
					.descriptorType = vk::DescriptorType::eUniformBuffer,
					.pBufferInfo = &skyBufferInfo
				};

				descriptorWriteSet.push_back(skyBufferSet);

				device->updateDescriptorSets(descriptorWriteSet, {});

			}
		}



	};

}

#endif