#include <monster/MonsterSlang.h>


void printDiagnostics(slang::IBlob* diagnostics) {
	if (diagnostics) {
		std::cerr << "SLANG ERROR : " << (const char*)diagnostics->getBufferPointer() << std::endl;
		throw std::runtime_error((const char*)diagnostics->getBufferPointer());
	}
}

uint32_t MonsterSlang::loadShader(const std::string& shaderName,std::filesystem::path& vertfilepath, std::filesystem::path& fragfilepath)
{
	vulkanUtils::Shader* shader = new vulkanUtils::Shader();
	shader->vertShaderFilePath = new std::filesystem::path(vertfilepath);
	shader->fragShaderFilePath = new std::filesystem::path(fragfilepath);

	shader->vertShadername = shaderName + "_vert";
	shader->fragShaderName = shaderName + "_frag";

	shader->vertCodeSlang = std::vector<char>();
	shader->fragCodeSlang = std::vector<char>();

	ResourceHandler::GetInstance()->readFileContents(vertfilepath, &shader->vertCodeSlang);
	ResourceHandler::GetInstance()->readFileContents(fragfilepath, &shader->fragCodeSlang);

	shaders.push_back(std::move(shader));
	return shaders.size() - 1;

}

void MonsterSlang::compileShaderFiles()
{
	// target
	slang::TargetDesc targetDesc{};
	targetDesc.format = SLANG_SPIRV;
	targetDesc.profile = globalSession->findProfile("spirv_1_4");

	// session
	slang::SessionDesc sessionDesc{};
	sessionDesc.targets = &targetDesc;
	sessionDesc.targetCount = 1;

	Slang::ComPtr<slang::ISession> session;
	if (SLANG_FAILED(globalSession->createSession(sessionDesc,session.writeRef())))
	{
		throw std::runtime_error("UNABLE TO CREATE SLANG RUNTIME SESSION");
	}

	for (auto& shader: shaders)
	{
		if (!shader->compiled)
		{
			shader->vertCodeSpv = std::move(compileSlangFile(shader->vertShadername, shader->vertCodeSlang, "main", *shader->vertShaderFilePath, session));
			shader->fragCodeSpv = std::move(compileSlangFile(shader->fragShaderName, shader->fragCodeSlang, "main", *shader->fragShaderFilePath, session));

			shader->compiled = true;

			shader->vertCodeSlang.clear();
			shader->fragCodeSlang.clear();

		}
	}

	

}

std::vector<uint8_t> MonsterSlang::compileSlangFile(
	const std::string& shadername, 
	const std::vector<char>& sourceCode, 
	const std::string& entryPointName, 
	const std::filesystem::path& shaderPath,
	Slang::ComPtr<slang::ISession> session
)
{
	// load from mem
	Slang::ComPtr<slang::IBlob> diagnosticBlob;

	std::string sourceString(sourceCode.begin(), sourceCode.end());

	slang::IModule* module = session->loadModuleFromSourceString(
		shadername.c_str(),
		shaderPath.string().c_str(),
		sourceString.c_str(),
		diagnosticBlob.writeRef()
	);

	if (!module)
	{
		printDiagnostics(diagnosticBlob);

	}

	// find entrypoint
	Slang::ComPtr<slang::IEntryPoint> entryPoint;
	if (SLANG_FAILED(module->findEntryPointByName(entryPointName.c_str(),entryPoint.writeRef())))
	{
		throw std::runtime_error("UNABLE TO FIND ENTRY POINT IN SHADER");
	}

	// linking

	slang::IComponentType* components[] = { module, entryPoint.get()};
	Slang::ComPtr<slang::IComponentType> program;

	if (SLANG_FAILED(session->createCompositeComponentType(components,2,program.writeRef(),diagnosticBlob.writeRef())))
	{
		printDiagnostics(diagnosticBlob);
	}

	Slang::ComPtr<slang::IBlob> spirv;
	if (SLANG_FAILED(program->getEntryPointCode(0,0,spirv.writeRef(),diagnosticBlob.writeRef())))
	{
		printDiagnostics(diagnosticBlob);
	}

	const uint8_t* ptr = (const uint8_t*)spirv->getBufferPointer();
	size_t size = spirv->getBufferSize();

	return std::vector<uint8_t>(ptr, ptr + size);

}


[[nodiscard]] vk::raii::ShaderModule MonsterSlang::createShaderModule(const std::vector<uint8_t>& code) const {

	vk::ShaderModuleCreateInfo shaderModuleCreateInfo{
		.codeSize = code.size() * sizeof(uint8_t),
		.pCode = reinterpret_cast<const uint32_t*>(code.data())
	};

	vk::raii::ShaderModule shaderModule{ vkMonsterStats->device, shaderModuleCreateInfo };
	return std::move(shaderModule);

}

[[nodiscard]] vk::raii::ShaderModule MonsterSlang::createShaderModule(const std::vector<char>& code) const {

	vk::ShaderModuleCreateInfo shaderModuleCreateInfo{
		.codeSize = code.size() * sizeof(char),
		.pCode = reinterpret_cast<const uint32_t*>(code.data())
	};

	vk::raii::ShaderModule shaderModule{ vkMonsterStats->device, shaderModuleCreateInfo };
	return std::move(shaderModule);

}

vk::raii::ShaderModule MonsterSlang::createShaderModule(const uint32_t* code, size_t codeSize) const
{
	vk::ShaderModuleCreateInfo shaderModuleCreateInfo{
		.codeSize = codeSize,
		.pCode = code
	};

	vk::raii::ShaderModule shaderModule{ vkMonsterStats->device, shaderModuleCreateInfo };
	return std::move(shaderModule);
}

void MonsterSlang::compileToShaderModule()
{
	for (auto& shader : MonsterSlang::shaders)
	{
		shader->vertexShader = createShaderModule(shader->vertCodeSpv);
		shader->fragmentShader = createShaderModule(shader->fragCodeSpv);
	}
}

template<typename T>
void MonsterSlang::setupShaderBuffers(const uint32_t& shaderIndex, T& unifromBufferObj)
{
	vulkanUtils::Shader* shader = shaders[shaderIndex];
	shader->uniformBufferIndex = createUniformBuffers(unifromBufferObj);

	std::filesystem::path& imagePath = "../../../src/monster/shaders/far_fog_tex.png";
	VulkanTexture* texture = createTextureImage(imagePath);
	createImageView(texture);
	createTextureSampler(texture);
	
	shader->descriptorSetIndex = createDescriptorSets(
		{ vkMemAlloc->uniformBuffers.at(shader->uniformBufferIndex.first) ,vkMemAlloc->uniformBuffers.at(shader->uniformBufferIndex.second) },
		1,
		*texture,
		2
	);

}

template <typename T>
std::pair<uint32_t, uint32_t> MonsterSlang::createUniformBuffers(T& uniformBufferObj)
{
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vk::DeviceSize bufferSize = sizeof(uniformBufferObj);

		auto [buffer, alloc] = createBuffer(bufferSize,
			vk::BufferUsageFlagBits::eUniformBuffer,
			VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
			VMA_MEMORY_USAGE_AUTO);
		vkMemAlloc->uniformBuffers.emplace_back(std::move(vk::raii::Buffer(vkMonsterStats->device, buffer)));
		vkMemAlloc->uniformBufferAlloc.emplace_back(std::move(alloc));
		vkMemAlloc->uniformBuffersMapped.emplace_back(std::move(alloc->GetMappedData()));

	}
	if (vkMemAlloc->uniformBuffersMapped.size() <= 1)
	{
		throw std::runtime_error("UNABLE TO CREATE UNIFROM BUFFERS");
	}
	return { vkMemAlloc->uniformBuffersMapped.size() - 1, vkMemAlloc->uniformBuffersMapped.size() - 2 };
}

uint32_t MonsterSlang::createDescriptorSets
(
	std::vector<vk::Buffer&> uniformBuffer,
	uint32_t& uboBinding,
	VulkanTexture& texture,
	uint32_t& textureBinding
)
{
	// set layout for descriptor Sets
	std::vector<vk::DescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, *vkDescriptors->descriptorSetLayout);
	vk::DescriptorSetAllocateInfo allocInfo{
		.descriptorPool = vkDescriptors->descriptorPool,
		.descriptorSetCount = static_cast<uint32_t>(layouts.size()),
		.pSetLayouts = layouts.data()
	};

	// Allocate Descriptor sets
	std::shared_ptr<MonsterDescriptors> descriptor = std::make_shared<MonsterDescriptors>();
	descriptor.reset(&vkMonsterStats->device.allocateDescriptorSets(allocInfo));
	descriptorSets.push_back(std::move(descriptor));

	//configure allocated descriptor sets
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vk::DescriptorBufferInfo bufferInfo{
			.buffer =  uniformBuffer[i],
			.offset = 0,
			.range = sizeof(UniformBufferObject)
		};

		vk::DescriptorImageInfo imageInfo{
			.sampler = texture.textureSampler,
			.imageView = texture.textureView,
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
		std::array<vk::WriteDescriptorSet, 2>descriptorWrites{ {
			{
			.dstSet = descriptorSets.back()->at(0),
			.dstBinding = uboBinding,
			.dstArrayElement = 0,
			.descriptorCount = 1,
			.descriptorType = vk::DescriptorType::eUniformBuffer,
			.pBufferInfo = &bufferInfo
			},
			{
			.dstSet = descriptorSets.back()->at(1),
			.dstBinding = textureBinding,
			.dstArrayElement = 0,
			.descriptorCount = 1,
			.descriptorType = vk::DescriptorType::eCombinedImageSampler,
			.pImageInfo = &imageInfo
			}
		} };

		vkMonsterStats->device.updateDescriptorSets(descriptorWrites, {});

		
	}

	if (descriptorSets.size() <= 0)
	{
		throw std::runtime_error("DESCRIPTOR SET FAILED TO CREATE!");
	}

	return descriptorSets.size() - 1;
}

std::shared_ptr<VulkanTexture> MonsterSlang::createTextureImage(
	std::filesystem::path& texturePath
)
{
	VulkanTexture* texture = new VulkanTexture();

	//std::filesystem::path filepath = std::filesystem::path("../../../src/vktest/textures/praise_the_sun.png");
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load(
		std::filesystem::absolute(texturePath).string().c_str(),
		&texWidth,
		&texHeight,
		&texChannels,
		STBI_rgb_alpha
	);
	vk::DeviceSize imageSize = texWidth * texHeight * 4;

	

	if (!pixels)
	{
		throw std::runtime_error("Failed to load image");
	}

	// staging buffer for texture
	auto [stagingBuffer, stagingAlloc] =
		createBuffer(imageSize, vk::BufferUsageFlagBits::eTransferSrc,
			VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
			VMA_MEMORY_USAGE_AUTO
		);

	void* data = nullptr;
	vmaMapMemory(vkMemAlloc->vmaAllocator, stagingAlloc, &data);
	memcpy(data, pixels, imageSize);
	vmaUnmapMemory(vkMemAlloc->vmaAllocator, stagingAlloc);

	//vmaCopyMemoryToAllocation(vkMemAlloc.vmaAllocator, pixels, stagingAlloc, 0, imageSize);

	std::tie(texture->texture, texture->alloc) = createImage(
		texWidth,
		texHeight,
		vkMonsterStats->swapChainSurfaceFormat.format,
		vk::ImageTiling::eOptimal,
		vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
		0,
		VMA_MEMORY_USAGE_AUTO
	);

	vk::raii::CommandBuffer commandBuffer = begineSingleTimeCommands();
	transitionImageLayout(commandBuffer, texture->texture, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
	copyBufferToImage(
		commandBuffer,
		stagingBuffer,
		texture->texture,
		static_cast<uint32_t>(texWidth),
		static_cast<uint32_t>(texHeight)
	);
	transitionImageLayout(commandBuffer, texture->texture, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);
	endSingleTimeCommands(std::move(commandBuffer));

	vmaDestroyBuffer(vkMemAlloc->vmaAllocator, stagingBuffer, stagingAlloc);

	stbi_image_free(pixels);
	std::shared_ptr<VulkanTexture> texture_ptr = std::shared_ptr<VulkanTexture>();
	texture_ptr.reset(texture);
	textures.push_back(std::move(texture_ptr));
	return textures.back();
}

void MonsterSlang::createTextureImageView(
	VulkanTexture* texture
)
{

	texture->textureView = createImageView(
		*texture->texture,
		vk::Format::eR8G8B8A8Srgb,
		{ vk::ImageAspectFlagBits::eColor }
	);

}

void MonsterSlang::createTextureSampler(VulkanTexture* texture)
{

	vk::PhysicalDeviceProperties properties = vkMonsterStats->gpuDevice.getProperties();

	vk::SamplerCreateInfo samplerInfo{
		.magFilter = vk::Filter::eLinear,
		.minFilter = vk::Filter::eLinear,
		.mipmapMode = vk::SamplerMipmapMode::eLinear,
		.addressModeU = vk::SamplerAddressMode::eRepeat,
		.addressModeV = vk::SamplerAddressMode::eRepeat,
		.addressModeW = vk::SamplerAddressMode::eRepeat,
		.anisotropyEnable = vk::True,
		.maxAnisotropy = properties.limits.maxSamplerAnisotropy,
		.compareEnable = vk::False,
		.compareOp = vk::CompareOp::eAlways,
		.borderColor = vk::BorderColor::eIntOpaqueBlack,
		.unnormalizedCoordinates = vk::False,
	};

	texture->textureSampler = vk::raii::Sampler(vkMonsterStats->device, samplerInfo);


}

void MonsterSlang::transitionImageLayout(vk::raii::CommandBuffer& commandBuffer, const vk::raii::Image& image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout)
{
	vk::ImageMemoryBarrier barrier{
		.oldLayout = oldLayout,
		.newLayout = newLayout,
		.srcQueueFamilyIndex = vk::QueueFamilyIgnored,
		.dstQueueFamilyIndex = vk::QueueFamilyIgnored,
		.image = image,
		.subresourceRange = {.aspectMask = vk::ImageAspectFlagBits::eColor, .levelCount = 1, .layerCount = 1}
	};

	vk::PipelineStageFlags sourceStage;
	vk::PipelineStageFlags destinationStage;

	if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal)
	{
		barrier.srcAccessMask = {};
		barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

		sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
		destinationStage = vk::PipelineStageFlagBits::eTransfer;
	}
	else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {

		barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

		sourceStage = vk::PipelineStageFlagBits::eTransfer;
		destinationStage = vk::PipelineStageFlagBits::eFragmentShader;

	}
	else {
		throw std::invalid_argument("unsupported layout transition!");
	}

	commandBuffer.pipelineBarrier(sourceStage, destinationStage, {}, {}, nullptr, barrier);
}

vk::raii::ImageView MonsterSlang::createImageView(vk::Image const& image, vk::Format format, vk::ImageAspectFlags flags)
{
	vk::ImageViewCreateInfo imageViewCreateInfo{
		.image = image,
		.viewType = vk::ImageViewType::e2D,
		.format = format,
		.subresourceRange = {
			.aspectMask = flags,
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1}
	};
	vk::raii::ImageView imageView = vk::raii::ImageView(vkMonsterStats->device, imageViewCreateInfo);
	return std::move(imageView);
}