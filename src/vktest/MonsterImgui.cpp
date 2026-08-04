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
