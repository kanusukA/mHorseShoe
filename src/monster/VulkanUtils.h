#ifndef VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
	#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#endif // !VULKAN_HPP_NO_STRUCT_CONSTRUCTORS



#include <vulkan/vulkan.hpp>
#include "vulkan/vulkan_core.h"

#include <vulkan/vulkan_raii.hpp>

#include <vk_mem_alloc.h>

#include <glm/glm.hpp>

#include <filesystem>

#ifndef VULKAN_UTILS
#define VULKAN_UTILS

constexpr int MAX_FRAMES_IN_FLIGHT = 2;

struct UniformBufferObject {
	/*glm::vec2 foo;
	alignas(16)*/ // YOU CAN ALSO USE GLM_FORCE_DEFAULT_ALIGNED_GENTYPES for consistent alignment but it does not work in nested struct
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

struct MonsterBuffer {
	vk::Buffer buffer;
	VmaAllocation alloc;
	void* bufferMapped;
	vk::DeviceSize bufferSize;
};

struct MonsterTexture {
	vk::raii::Image textureImage = nullptr;
	vk::raii::ImageView textureImageView = nullptr;
	vk::raii::Sampler textureSampler = nullptr;
	VmaAllocation textureAlloc;
};

struct MonsterPipe {
	vk::raii::Pipeline graphicsPipeline = nullptr;
	vk::raii::PipelineLayout descriptorPipeLayout = nullptr;
	std::vector<vk::raii::DescriptorSets> descritorSets {};
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

	struct Shader {

		std::filesystem::path* vertShaderFilePath = nullptr;
		std::filesystem::path* fragShaderFilePath = nullptr;

		vk::raii::ShaderModule vertexShader = nullptr;
		vk::raii::ShaderModule fragmentShader = nullptr;

		uint32_t graphicsPipelineIndex;

		uint32_t tUBOIndex;

		std::vector<MonsterBuffer> transformBuffers;

		MonsterPipe monsterPipe = MonsterPipe();

		MonsterTexture monsterTexture = MonsterTexture(); // init consistent

		virtual std::vector<vk::DescriptorSetLayoutBinding> getBindings() {
			
			return
			{
				vk::DescriptorSetLayoutBinding{
					.binding = 0,
					.descriptorType = vk::DescriptorType::eUniformBuffer,
					.descriptorCount = 1,
					.stageFlags = vk::ShaderStageFlagBits::eVertex
				},
				vk::DescriptorSetLayoutBinding{
					.binding = 1,
					.descriptorType = vk::DescriptorType::eCombinedImageSampler,
					.descriptorCount = 1,
					.stageFlags = vk::ShaderStageFlagBits::eFragment
				}
			};
			
		}

		virtual void updateDescriptorWrites(vk::raii::Device* device) {

			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			{
				std::vector<vk::WriteDescriptorSet> descriptorWrites{};
				/*vk::DescriptorBufferInfo bufferInfo{
					.buffer = vkMemAlloc.uniformBuffers[importedMeshes[meshIndex]->shaders.tUBOIndex + i],
					.offset = 0,
					.range = sizeof(UniformBufferObject)
				};*/

				vk::DescriptorBufferInfo bufferInfo{
					.buffer = transformBuffers.at(i).buffer,
					.offset = 0,
					.range = sizeof(UniformBufferObject)
				};

				vk::DescriptorImageInfo imageInfo{
					.sampler = monsterTexture.textureSampler,
					.imageView = monsterTexture.textureImageView,
					.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal
				};

				/*
				vk::WriteDescriptorSet descriptorWrite{
					.dstSet = vkDescriptors.descriptorSets[i],
					.dstBinding = 0,
					.dstArrayElement = 0,
					.descriptorCount = 1,
					.descriptorType = vk::DescriptorType::eUniformBuffer,
					.pBufferInfo = &bufferInfo
				};
				*/
				descriptorWrites.push_back({
					.dstSet = monsterPipe.descritorSets.front()[i],
					.dstBinding = 0,
					.dstArrayElement = 0,
					.descriptorCount = 1,
					.descriptorType = vk::DescriptorType::eUniformBuffer,
					.pBufferInfo = &bufferInfo
					});

				descriptorWrites.push_back(
					{
					.dstSet = monsterPipe.descritorSets.front()[i],
					.dstBinding = 1,
					.dstArrayElement = 0,
					.descriptorCount = 1,
					.descriptorType = vk::DescriptorType::eCombinedImageSampler,
					.pImageInfo = &imageInfo
					}
				);

				device->updateDescriptorSets(descriptorWrites, {});

			}
			
		}
		
		// descriptors
		uint32_t descriptorPipeLayout;
		uint32_t descriptorSetLayout;
		uint32_t descriptorSets;


	};

}

#endif