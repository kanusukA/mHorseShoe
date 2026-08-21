#ifndef VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
	#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#endif // !VULKAN_HPP_NO_STRUCT_CONSTRUCTORS


#include <vulkan/vulkan.hpp>
#include "vulkan/vulkan_core.h"

#include <vulkan/vulkan_raii.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <filesystem>

#ifndef VULKAN_UTILS
#define VULKAN_UTILS

struct UniformBufferObject {
	/*glm::vec2 foo;
	alignas(16)*/ // YOU CAN ALSO USE GLM_FORCE_DEFAULT_ALIGNED_GENTYPES for consistent alignment but it does not work in nested struct
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

// buffer
struct MonsterBuffer {
	std::vector<vk::raii::Buffer> buffers{};
	std::vector<void*> bufferMapped{};
	std::vector<vk::DeviceSize> bufferSizes{};
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

		//std::vector<uint32_t> transformBufferIndexes{};
		MonsterBuffer transformBuffer;

		std::vector<uint32_t> uniformBufferIndexes{};
		uint32_t pipelineLayoutIndex; // SHOULD BE SIMILAR TO THAT OF GRAPHICS PIPELINE
		uint32_t descriptorSetLayoutIndex;
		uint32_t descriptorSetIndex;

		void updateUboBuffer(
			const glm::vec3& position,
			const glm::vec3& rotation,
			float radiance,
			const glm::vec3& scale,
			const glm::vec3& camPos,
			const glm::vec3& camFront,
			const glm::vec3& camUp,
			float width,
			float height
		) {
			UniformBufferObject ubo{};

			ubo.model = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), radiance, rotation) * glm::scale(glm::mat4(1.0f), scale);

			ubo.view = glm::lookAt(camPos, camPos + camFront, camUp);

			ubo.proj = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 1000.0f);
			//ubo.proj[1][1] *= -1;
			for (auto tBuffer: transformBuffer.bufferMapped)
			{
				memcpy(tBuffer, &ubo, sizeof(ubo));
			}
			
		}


	};

}

#endif