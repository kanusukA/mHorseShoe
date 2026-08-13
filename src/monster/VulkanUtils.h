#ifndef VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
	#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#endif // !VULKAN_HPP_NO_STRUCT_CONSTRUCTORS




#ifndef VULKAN_UTILS
#define VULKAN_UTILS

#include <vulkan/vulkan.hpp>
#include "vulkan/vulkan_core.h"

#include <vulkan/vulkan_raii.hpp>

#include <glm/glm.hpp>

#include <filesystem>

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

		bool compiled = false;

		uint32_t shaderIndex;
		uint32_t graphicsPipelineIndex;

		std::filesystem::path* vertShaderFilePath = nullptr;
		std::filesystem::path* fragShaderFilePath = nullptr;

		// SLANG CODE IS RETAINED UNTIL THE SPV IS NOT COMPILED AND IS LATER CLEARED;
		std::vector<char> vertCodeSlang;
		std::vector<char> fragCodeSlang;

		std::vector<uint8_t> vertCodeSpv;
		std::vector<uint8_t> fragCodeSpv;

		std::string vertShadername;
		std::string fragShaderName;

		vk::raii::ShaderModule vertexShader = nullptr;
		vk::raii::ShaderModule fragmentShader = nullptr;

		


	};

}

#endif