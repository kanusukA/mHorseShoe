#pragma once


#include <stdio.h>
#include <filesystem>

//#include <monster/VulkanUtils.h>
#include <monster/MonsterBuffer.h>

#include <slang/slang.h>
#include <slang/slang-com-ptr.h>



class MonsterSlang : public MonsterBuffer {

	Slang::ComPtr<slang::IGlobalSession> globalSession;
	VulkanMemAlloc* vkMemAlloc;
	VulkanDescriptors* vkDescriptors;
	VulkanStatus* vkMonsterStats;

public:

	// each discriptor has two descriptors for the frame in flight calculation.
	std::vector<std::shared_ptr<vk::raii::DescriptorSets>> descriptorSets = std::vector<std::shared_ptr<vk::raii::DescriptorSets>>();
	std::vector<std::shared_ptr<VulkanTexture>> textures = std::vector<std::shared_ptr<VulkanTexture>>();
	std::vector<std::shared_ptr<vulkanUtils::Shader>> shaders = std::vector<std::shared_ptr<vulkanUtils::Shader>>();


	void InitMonsterSlang(VulkanStatus* vkMonsterStats_p, VulkanDescriptors* vkDescriptors_p, VulkanMemAlloc* vkMemAlloc_p) {
		vkMonsterStats = vkMonsterStats_p;
		vkDescriptors = vkDescriptors_p;
		vkMemAlloc = vkMemAlloc_p;
		if (SLANG_FAILED(slang::createGlobalSession(globalSession.writeRef())))
		{
			throw std::runtime_error("Unable to create Slang Session");
		}
	}

	std::shared_ptr<vulkanUtils::Shader> loadShader(const std::string& shadername,std::filesystem::path& vertfilepath, std::filesystem::path& fragfilepath);

	void compileShaderFiles();

	std::vector<uint8_t> compileSlangFile(
		const std::string& shadername,
		const std::vector<char>& sourceCode, 
		const std::string& entryPointName, 
		const std::filesystem::path& shaderPath,
		Slang::ComPtr<slang::ISession> session
	);

	vk::raii::ShaderModule createShaderModule(const std::vector<char>& code) const;
	vk::raii::ShaderModule createShaderModule(const std::vector<uint8_t>& code) const;
	vk::raii::ShaderModule createShaderModule(const uint32_t* code, size_t codeSize) const;

	void compileShaders();
	
	void setupShaderBuffers(std::weak_ptr<vulkanUtils::Shader> shader,const vk::DeviceSize& bufferSize);

	// Buffers for shader
	std::shared_ptr<MBuffer> createUniformBuffers(const vk::DeviceSize& size);


	/*std::shared_ptr<vk::raii::DescriptorSets> createDescriptorSets(
		std::vector<std::pair<vk::Buffer, vk::ShaderStageFlags>> uniformBuffer,
		const vk::DeviceSize& bufferSize,
		std::vector<uint32_t> uboBinding,
		VulkanTexture& texture,
		const uint32_t& textureBinding
	);*/


	std::shared_ptr<VulkanTexture> createTextureImage(std::filesystem::path& texturePath);
	void createTextureImageView(VulkanTexture* texture);
	void createTextureSampler(VulkanTexture* texture);

	vk::raii::ImageView createImageView(vk::Image const& image, vk::Format format, vk::ImageAspectFlags flags);

	void transitionImageLayout(
		vk::raii::CommandBuffer& commandBuffer,
		const vk::raii::Image& image,
		vk::ImageLayout oldLayout,
		vk::ImageLayout newLayout
	);
	

};