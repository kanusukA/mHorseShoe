#include <Monster.h>

void Monster::InitImgui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void) io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();

	ImGui_ImplSDL3_InitForVulkan(sdlWindow);

	ImGui_ImplVulkan_PipelineInfo pipelineInfo{
		
		.Subpass = 0,
		.MSAASamples = VK_SAMPLE_COUNT_1_BIT,
		
	};

	ImGui_ImplVulkan_InitInfo init_info = {
		.ApiVersion = VK_API_VERSION_1_3,
		.Instance = *vkMonsterStats.vkInstance,
		.PhysicalDevice = *vkMonsterStats.gpuDevice,
		.Device = *vkMonsterStats.device,
		.QueueFamily = vkMonsterStats.queueIndex,
		.Queue = *vkMonsterStats.graphicsQueue,
		.DescriptorPoolSize = IMGUI_IMPL_VULKAN_MINIMUM_SAMPLED_IMAGE_POOL_SIZE + IMGUI_IMPL_VULKAN_MINIMUM_SAMPLER_POOL_SIZE + MAX_FRAMES_IN_FLIGHT,
		//.DescriptorPool = *vkDescriptors.descriptorPool,
		.ImageCount = MAX_FRAMES_IN_FLIGHT,
		.PipelineCache = VK_NULL_HANDLE,
		.PipelineInfoMain = pipelineInfo,
		
		
	};

	ImGui_ImplVulkan_Init(&init_info);

	createImGuiShaders();


}


void Monster::startImGuiFrame()
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
}

void Monster::renderFrame()
{
	ImGui::Render();
	ImDrawData* drawData = ImGui::GetDrawData();
	const bool isMinimized = (drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f);
	if (!isMinimized)
	{
		renderVulkanFrame();

	}
}

void Monster::ShutdownImGui()
{
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
}


void Monster::updateTexture(vk::raii::CommandBuffer& commandBuffer, ImTextureData* tex)
{
	if (tex->Status == ImTextureStatus_WantCreate || tex->Status == ImTextureStatus_WantUpdates)
	{
		uint16_t texWidth = tex->Width;
		uint16_t texHeight = tex->Height;
		unsigned char* fontData = (unsigned char*)tex->Pixels;

		if (!fontData)
		{
			return;
		}

		vk::DeviceSize uploadSize = texWidth * texHeight * tex->BytesPerPixel;
		vk::Format format = (tex->BytesPerPixel == 4) ? vk::Format::eR8G8B8A8Unorm : vk::Format::eR8Unorm;

		if (tex->Status == ImTextureStatus_WantCreate)
		{
			vk::Extent3D extent{ static_cast<uint32_t>(texWidth),static_cast<uint32_t>(texHeight), 1 };

			vk::ImageCreateInfo imageCreateInfo{};
			imageCreateInfo.extent = extent;
			imageCreateInfo.format = format;
			imageCreateInfo.usage = vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferDst;


			fontImage = device->createImage(imageCreateInfo);

			vk::ImageViewCreateInfo imageViewCreateInfo{};
			imageViewCreateInfo.image = fontImage;
			imageViewCreateInfo.format = format;

			fontImageView = device->createImageView(imageViewCreateInfo);

		}

		vk::BufferCreateInfo bufferInfo{
		.size = uploadSize,
		.usage = vk::BufferUsageFlagBits::eTransferSrc,
		.sharingMode = vk::SharingMode::eExclusive
		};

		VmaAllocationCreateInfo allocInfo{
			.flags = VmaAllocationCreateFlagBits::VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
			.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO

		};

		VkBuffer stagingBuffer;
		VmaAllocation allocation;

		std::tie(stagingBuffer, allocation) = createBuffer(uploadSize,
			vk::BufferUsageFlagBits::eTransferSrc,
			VmaAllocationCreateFlagBits::VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
			VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO
		);

		vmaCopyMemoryToAllocation(allocator, fontData, allocation, 0, uploadSize);

		vk::raii::CommandBuffer cmdBuffer = begineSingleTimeCommands();

		transitionImageLayout(cmdBuffer, fontImage, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
		copyBufferToImage(cmdBuffer, stagingBuffer, fontImage, texWidth, texHeight);
		transitionImageLayout(cmdBuffer, fontImage, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);

		endSingleTimeCommands(std::move(cmdBuffer));

		tex->SetTexID((ImTextureID)(intptr_t)(VkDescriptorSet)*descriptorSet);
		tex->SetStatus(ImTextureStatus_OK);



	}
}

void Monster::setUtils(vk::raii::Device& p_device, vk::raii::PhysicalDevice& p_physicalDevice, vk::raii::Queue& p_graphicsQueue, uint32_t p_graphicsQueueFamily, VmaAllocator p_allocator)
{
	device = &p_device;
	physicalDevice = &p_physicalDevice;
	graphicsQueue = &p_graphicsQueue;
	graphicsQueueFamily = p_graphicsQueueFamily;
	allocator = p_allocator;

	
	
		vertexBuffers = device->createBuffer({ .usage = vk::BufferUsageFlagBits::eVertexBuffer });
		indexBuffers = device->createBuffer({ .usage = vk::BufferUsageFlagBits::eIndexBuffer });

		

	

	renderingInfo.colorAttachmentCount = 1;
	vk::Format formats[] = { colorFormat };
	renderingInfo.pColorAttachmentFormats = &colorFormat;

	// Command buffer
	vk::CommandBufferAllocateInfo cmdBufferAllocInfo{
	    .commandPool = vkMonsterStats.commandPool,
	    .level = vk::CommandBufferLevel::ePrimary,
		.commandBufferCount = 1,
		
	};
	commandBuffer = std::move(vk::raii::CommandBuffers(vkMonsterStats.device,cmdBufferAllocInfo).front());
}

void Monster::updateBuffers(uint32_t frameIndex)
{
	ImDrawData* drawData = ImGui::GetDrawData();
	if (!drawData || drawData->CmdListsCount == 0)
	{
		return;
	}

	vk::DeviceSize vertexBufferSize = drawData->TotalVtxCount * sizeof(ImDrawVert);
	vk::DeviceSize indexBufferSize = drawData->TotalIdxCount * sizeof(ImDrawIdx);

	VmaAllocationInfo vertAllocInfo = {};
	VmaAllocationInfo indxAllocInfo = {};

	if (drawData->TotalVtxCount > vertexCount)
	{

		std::tie(vertexBuffers, vertexAllocation) = createBuffer(
			vertexBufferSize,
			vk::BufferUsageFlagBits::eVertexBuffer,
			VmaAllocationCreateFlagBits::VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VmaAllocationCreateFlagBits::VMA_ALLOCATION_CREATE_MAPPED_BIT,
			VMA_MEMORY_USAGE_AUTO,
			&vertAllocInfo
		);

		vertexCount = drawData->TotalVtxCount;

	}

	if (drawData->TotalIdxCount > indexCount)
	{
		std::tie(indexBuffers, indexAllocation) = createBuffer(
			indexBufferSize,
			vk::BufferUsageFlagBits::eIndexBuffer,
			VmaAllocationCreateFlagBits::VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VmaAllocationCreateFlagBits::VMA_ALLOCATION_CREATE_MAPPED_BIT,
			VMA_MEMORY_USAGE_AUTO,
			&indxAllocInfo
		);
		 
		indexCount = drawData->TotalIdxCount;
	}



	ImDrawVert* vtxDst = static_cast<ImDrawVert*>(vertAllocInfo.pMappedData);
	ImDrawIdx* idxDst = static_cast<ImDrawIdx*>(indxAllocInfo.pMappedData);

	for (size_t i = 0; i < drawData->CmdListsCount; i++)
	{
		const ImDrawList* cmdList = drawData->CmdLists[i];
		
		memcpy(vtxDst, cmdList->VtxBuffer.Data, cmdList->VtxBuffer.Size * sizeof(ImDrawVert));
		memcpy(idxDst, cmdList->IdxBuffer.Data, cmdList->IdxBuffer.Size * sizeof(ImDrawIdx));

		vtxDst += cmdList->VtxBuffer.Size;
		idxDst += cmdList->IdxBuffer.Size;
	}

	vmaFlushAllocation(allocator, vertexAllocation, 0, VK_WHOLE_SIZE);
	vmaFlushAllocation(allocator, indexAllocation, 0, VK_WHOLE_SIZE);

}

void Monster::createImGuiShaders()
{
	if (imguiVertShader == VK_NULL_HANDLE)
	{
		imguiVertShader = createShaderModule(__glsl_shader_vert_spv,sizeof(__glsl_shader_vert_spv));
	}
	if (imguiFragShader == VK_NULL_HANDLE)
	{
		imguiFragShader = createShaderModule(__glsl_shader_frag_spv, sizeof(__glsl_shader_frag_spv));
	}
}

void Monster::createImGuiPipeline()
{
	vk::PipelineShaderStageCreateInfo vertexShaderStageCreateInfo{ .stage = vk::ShaderStageFlagBits::eVertex, .module = imguiVertShader, .pName = "main" };

	vk::PipelineShaderStageCreateInfo fragShaderStageCreateInfo{ .stage = vk::ShaderStageFlagBits::eFragment, .module = imguiFragShader, .pName = "main" };

	vk::PipelineShaderStageCreateInfo ShaderStages[] = { vertexShaderStageCreateInfo,fragShaderStageCreateInfo };

	vk::VertexInputBindingDescription binding_desc[1] = {};
	binding_desc[0].stride = sizeof(ImDrawVert);
	binding_desc[0].inputRate = vk::VertexInputRate::eVertex;

	vk::VertexInputAttributeDescription attribute_desc[3] = {};
	attribute_desc[0].location = 0;
	attribute_desc[0].binding = binding_desc[0].binding;
	attribute_desc[0].format = vk::Format::eR32G32Sfloat;
	attribute_desc[0].offset = offsetof(ImDrawVert, pos);
	attribute_desc[1].location = 1;
	attribute_desc[1].binding = binding_desc[0].binding;
	attribute_desc[1].format = vk::Format::eR32G32Sfloat;
	attribute_desc[1].offset = offsetof(ImDrawVert, uv);
	attribute_desc[2].location = 2;
	attribute_desc[2].binding = binding_desc[0].binding;
	attribute_desc[2].format = vk::Format::eR8G8B8A8Unorm;
	attribute_desc[2].offset = offsetof(ImDrawVert, uv);

	vk::PipelineVertexInputStateCreateInfo vertexInfo = {};
	vertexInfo.vertexBindingDescriptionCount = 1;
	vertexInfo.vertexAttributeDescriptionCount = 3;
	vertexInfo.pVertexBindingDescriptions = binding_desc;
	vertexInfo.pVertexAttributeDescriptions = attribute_desc;

	vk::PipelineInputAssemblyStateCreateInfo ia_info = {};
	ia_info.topology = vk::PrimitiveTopology::eTriangleList;

	vk::PipelineViewportStateCreateInfo viewport_info = {};
	viewport_info.viewportCount = 1;
	viewport_info.scissorCount = 1;

	vk::PipelineRasterizationStateCreateInfo raster_info = {};
	raster_info.polygonMode = vk::PolygonMode::eFill;
	raster_info.cullMode = vk::CullModeFlagBits::eNone;
	raster_info.frontFace = vk::FrontFace::eCounterClockwise;
	raster_info.lineWidth = 1.0f;

	vk::PipelineMultisampleStateCreateInfo ms_info = {};
	ms_info.rasterizationSamples = vk::SampleCountFlagBits::e1;

	vk::PipelineColorBlendAttachmentState color_attachment[1] = {};
	color_attachment[0].blendEnable = VK_TRUE;
	color_attachment[0].srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
	color_attachment[0].dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
	color_attachment[0].colorBlendOp = vk::BlendOp::eAdd;
	color_attachment[0].srcAlphaBlendFactor = vk::BlendFactor::eOne;
	color_attachment[0].dstAlphaBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
	color_attachment[0].alphaBlendOp = vk::BlendOp::eAdd;
	color_attachment[0].colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;


	vk::PipelineDepthStencilStateCreateInfo depth_info = {};

	vk::PipelineColorBlendStateCreateInfo blend_info = {};
	blend_info.attachmentCount = 1;
	blend_info.pAttachments = color_attachment;

	std::vector<vk::DynamicState> dynamicStates{ vk::DynamicState::eViewport, vk::DynamicState::eScissor };
	vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo{ .dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()), .pDynamicStates = dynamicStates.data() };

	vk::PipelineRenderingCreateInfo renderingCreateInfo{
		.colorAttachmentCount = 1,
		.pColorAttachmentFormats = &vkMonsterStats.swapChainSurfaceFormat.format,
		.depthAttachmentFormat = vkTextures.depthFormat
	};

	vk::GraphicsPipelineCreateInfo create_info = {};
	create_info.stageCount = 2;
	create_info.pStages = ShaderStages;
	create_info.pVertexInputState = &vertexInfo;
	create_info.pInputAssemblyState = &ia_info;
	create_info.pViewportState = &viewport_info;
	create_info.pRasterizationState = &raster_info;
	create_info.pMultisampleState = &ms_info;
	create_info.pDepthStencilState = &depth_info;
	create_info.pColorBlendState = &blend_info;
	create_info.pDynamicState = &dynamicStateCreateInfo;
	create_info.layout = pipelineLayout;
	create_info.renderPass = nullptr;

	vk::StructureChain<vk::GraphicsPipelineCreateInfo, vk::PipelineRenderingCreateInfo> pipelineCreateInfoChain = {
		create_info,
		renderingCreateInfo
	};

	pipeline = device->createGraphicsPipeline(pipelineCache, pipelineCreateInfoChain.get<vk::GraphicsPipelineCreateInfo>());
}


