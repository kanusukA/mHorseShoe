#ifndef VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
	#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#endif // !VULKAN_HPP_NO_STRUCT_CONSTRUCTORS



#include <vulkan/vulkan.hpp>
#include "vulkan/vulkan_core.h"

#include <vulkan/vulkan_raii.hpp>

#include <vk_mem_alloc.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

struct ColorBufferObject {
	glm::vec3 color;
};


struct SkyTexBufferObject {
	float offset;
	float smoothness;
};

struct PushConstObject {
	float time;
};

struct SkyBufferObject {
	
	
	/*float time;
	alignas(16)
	float windSpeed;
	alignas(16)*/
	float highlightOffset = 0.421f;
	/*alignas(16)*/
	float highlightSmoothness = 0.236f;
	//alignas(16)
	float middleOffset = 0.584f;
	//alignas(16)
	float middleSmoothness = 0.067f;
	//alignas(16)
	float coreOffset = 0.638f;
	//alignas(16)
	float coreSmoothness = 0.231f;
	//alignas(16)
	float bumpOffset;
	//alignas(16)
	float bumprange;
	//alignas(16)
	float bumpHeight;

	float starOffset;

	alignas(16)
	glm::vec4 baseColor = glm::vec4(0.247f,0.0f,0.205f,1.0f);
	glm::vec4 highlightColor = glm::vec4(0.07f,0.063f,0.182f,1.0f);
	glm::vec4 middleColor = glm::vec4(0.039f,0.003f,0.237f,1.0f);
	glm::vec4 coreColor = glm::vec4(0.0f,0.0f,0.055f,1.0f);
	

};

struct MonsterBuffer {
	vk::Buffer buffer;
	VmaAllocation alloc;
	VmaAllocationInfo allocInfo;
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
	vk::raii::DescriptorSetLayout descriptorSetLayout = nullptr;
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

	class Shader
	{
	public:

		virtual void _updateDescriptorWrites(
			vk::raii::Device* device,
			const std::vector<MonsterBuffer>& buffer,
			const vk::raii::DescriptorSets& sets,
			const std::vector<MonsterBuffer>& fragBuf,
			vk::DeviceSize fragBufSize
		) {

			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			{
				std::vector<vk::WriteDescriptorSet> descriptorWrites{};

				vk::DescriptorBufferInfo bufferInfo{
					.buffer = buffer.at(i).buffer,
					.offset = vk::DeviceSize(0),
					.range = sizeof(UniformBufferObject)
				};

				vk::DescriptorBufferInfo buffer2Info{
					.buffer = fragBuf.at(i).buffer,
					.offset = vk::DeviceSize(0),
					.range = fragBufSize
				};
				

				descriptorWrites.push_back({
					.dstSet = sets[i],
					.dstBinding = 0,
					.dstArrayElement = 0,
					.descriptorCount = 1,
					.descriptorType = vk::DescriptorType::eUniformBuffer,
					.pBufferInfo = &bufferInfo
					});

				descriptorWrites.push_back(
					{
					.dstSet = sets[i],
					.dstBinding = 1,
					.dstArrayElement = 0,
					.descriptorCount = 1,
					.descriptorType = vk::DescriptorType::eUniformBuffer,
					.pBufferInfo = &buffer2Info
					}
				);

				uint32_t texIndex = 2;

				for (auto& texture: textures)
				{
					vk::DescriptorImageInfo imageInfo{
					.sampler = texture.textureSampler,
					.imageView = texture.textureImageView,
					.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal
					};

					descriptorWrites.push_back(
						{
						.dstSet = sets[i],
						.dstBinding = texIndex,
						.dstArrayElement = 0,
						.descriptorCount = 1,
						.descriptorType = vk::DescriptorType::eCombinedImageSampler,
						.pImageInfo = &imageInfo
						}
					);

					texIndex += 1;

				}
				

				device->updateDescriptorSets(descriptorWrites, {});

			}

		}

		bool shaderLoaded = false;

		bool colorBlending = false;

		vk::PolygonMode mode = vk::PolygonMode::eFill;

		std::filesystem::path* vertShaderFilePath = nullptr;
		std::filesystem::path* fragShaderFilePath = nullptr;

		vk::raii::ShaderModule vertexShader = nullptr;
		vk::raii::ShaderModule fragmentShader = nullptr;

		std::string vertShaderName;
		std::string fragShaderName;

		MonsterPipe monsterPipe = MonsterPipe();

		std::vector<MonsterTexture> textures{};

		//MonsterTexture monsterTexture = MonsterTexture();

		virtual std::vector<vk::DescriptorSetLayoutBinding> getBindings() {
			std::vector<vk::DescriptorSetLayoutBinding> binds{};
			binds.push_back(vk::DescriptorSetLayoutBinding{
					.binding = 0,
					.descriptorType = vk::DescriptorType::eUniformBuffer,
					.descriptorCount = 1,
					.stageFlags = vk::ShaderStageFlagBits::eVertex
				});
			binds.push_back(
				vk::DescriptorSetLayoutBinding{
					.binding = 1,
					.descriptorType = vk::DescriptorType::eUniformBuffer,
					.descriptorCount = 1,
					.stageFlags = vk::ShaderStageFlagBits::eFragment
				}
			);

			for (uint32_t i = 0; i < textures.size(); i++)
			{
				binds.push_back(
					vk::DescriptorSetLayoutBinding{
					.binding = 2 + i,
					.descriptorType = vk::DescriptorType::eCombinedImageSampler,
					.descriptorCount = 1,
					.stageFlags = vk::ShaderStageFlagBits::eFragment
					}
				);
			}

			return binds;
			
		}


	};

	// skyBoxshader
	class SkyBoxShader : public Shader {
	public:

		/*std::vector<vk::DescriptorSetLayoutBinding> getBindings() override{

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
				},
				vk::DescriptorSetLayoutBinding{
					.binding = 2,
					.descriptorType = vk::DescriptorType::eUniformBuffer,
					.descriptorCount = 1,
					.stageFlags = vk::ShaderStageFlagBits::eFragment
				}
			};

		}*/

	};


}

#endif