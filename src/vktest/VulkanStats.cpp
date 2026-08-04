#include <VulkanStats.h>




ImGuiVulkanUtil::~ImGuiVulkanUtil()
{
	device->waitIdle();
}

void ImGuiVulkanUtil::init(float width, float height)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;

	io.BackendFlags |= ImGuiBackendFlags_RendererHasTextures;

	io.DisplaySize = ImVec2(width, height);
	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

	vulkanStyle = ImGui::GetStyle();
	vulkanStyle.Colors[ImGuiCol_TitleBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.6f);
	vulkanStyle.Colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 0.0f, 0.0f, 0.8f);
	vulkanStyle.Colors[ImGuiCol_MenuBarBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
	vulkanStyle.Colors[ImGuiCol_Header] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
	vulkanStyle.Colors[ImGuiCol_CheckMark] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);

	setStyle(0);
	
}

void ImGuiVulkanUtil::setStyle(uint32_t index) {
	ImGuiStyle& style = ImGui::GetStyle();

	switch (index) {
	case 0:
		// Custom Vulkan style
		style = vulkanStyle;
		break;
	case 1:
		// Classic style
		ImGui::StyleColorsClassic();
		break;
	case 2:
		// Dark style
		ImGui::StyleColorsDark();
		break;
	case 3:
		// Light style
		ImGui::StyleColorsLight();
		break;
	}
}

void ImGuiVulkanUtil::initResources()
{
	vk::SamplerCreateInfo samplerInfo{};
	samplerInfo.magFilter = vk::Filter::eLinear;
	samplerInfo.minFilter = vk::Filter::eLinear;
	samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
	samplerInfo.addressModeU = vk::SamplerAddressMode::eClampToEdge;
	samplerInfo.addressModeV = vk::SamplerAddressMode::eClampToEdge;
	samplerInfo.addressModeW = vk::SamplerAddressMode::eClampToEdge;
	samplerInfo.borderColor = vk::BorderColor::eFloatOpaqueWhite;

	sampler = device->createSampler(samplerInfo);

	// DescriptorPool
	vk::DescriptorPoolSize poolSize{
		.type = vk::DescriptorType::eCombinedImageSampler,
		.descriptorCount = 1
	};

	vk::DescriptorPoolCreateInfo poolInfo{};
	poolInfo.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
	poolInfo.maxSets = 2;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;

	descriptorPool = device->createDescriptorPool(poolInfo);

	vk::DescriptorSetLayoutBinding binding{};
	binding.descriptorType = vk::DescriptorType::eCombinedImageSampler;
	binding.descriptorCount = 1;
	binding.stageFlags = vk::ShaderStageFlagBits::eFragment;
	binding.binding = 0;

	vk::DescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &binding;

	descriptorSetLayout = device->createDescriptorSetLayout(layoutInfo);

	vk::DescriptorSetAllocateInfo allocInfo{};
	allocInfo.descriptorPool = *descriptorPool;
	allocInfo.descriptorSetCount = 1;
	vk::DescriptorSetLayout layouts[] = { *descriptorSetLayout };
	allocInfo.pSetLayouts = layouts;

	descriptorSet = std::move(device->allocateDescriptorSets(allocInfo).front());

	vk::DescriptorImageInfo imageInfo{};
	imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	imageInfo.imageView = fontImageView;
	imageInfo.sampler = *sampler;

	vk::WriteDescriptorSet writeSet{};
	writeSet.dstSet = *descriptorSet;
	writeSet.descriptorCount = 1;
	writeSet.descriptorType = vk::DescriptorType::eCombinedImageSampler;
	writeSet.pImageInfo = &imageInfo;
	writeSet.dstBinding = 0;

	device->updateDescriptorSets(writeSet, nullptr);

	vk::PipelineCacheCreateInfo pipelineCacheCreateInfo{};
	pipelineCache = device->createPipelineCache(pipelineCacheCreateInfo);

	vk::PushConstantRange pushConstantRange{
	.stageFlags = vk::ShaderStageFlagBits::eVertex,
	.offset = 0,
	.size = sizeof(PushConstBlock)
	};

	vk::PipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.setLayoutCount = 1;
	vk::DescriptorSetLayout setLayouts[] = { *descriptorSetLayout };
	pipelineLayoutInfo.pSetLayouts = setLayouts;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

	pipelineLayout = device->createPipelineLayout(pipelineLayoutInfo);

	

}

bool ImGuiVulkanUtil::newFrame()
{
	ImGui::NewFrame();

	ImGui::Begin("DEMO");
	ImGui::Text("Hello, Vulkan!");
	if (ImGui::Button("Click me!")) {
		// Handle button click
	}
	ImGui::End();

	ImGui::EndFrame();

	ImGui::Render();

	ImDrawData* drawData = ImGui::GetDrawData();

	if (drawData && drawData->CmdListsCount > 0)
	{
		if (drawData->TotalVtxCount > vertexCount || drawData->TotalIdxCount > indexCount)
		{
			needsUpdateBuffers = true;
			return true;
		}
	}
	return false;
}

void ImGuiVulkanUtil::drawFrame(vk::raii::CommandBuffer& commandBuffer)
{
	ImDrawData* drawData = ImGui::GetDrawData();
	if (!drawData || drawData->CmdListsCount == 0)
	{
		return;
	}

	if (drawData->Textures)
	{
		for (size_t i = 0; i < drawData->Textures->Size; i++)
		{
			ImTextureData* tex = (*drawData->Textures)[i];
			if (tex->Status != ImTextureStatus_OK)
			{
				updateTexture(commandBuffer, tex);
			}
		}
	}

	vk::RenderingAttachmentInfo colorAttachment{};

	vk::RenderingInfo renderingInfo{};
	renderingInfo.renderArea = vk::Rect2D({ 0, 0 }, {
		static_cast<uint32_t>(drawData->DisplaySize.x),
		static_cast<uint32_t>(drawData->DisplaySize.y)
		});

	renderingInfo.layerCount = 1;
	renderingInfo.colorAttachmentCount = 1;
	renderingInfo.pColorAttachments = &colorAttachment;

	commandBuffer.beginRendering(renderingInfo);

	// Create Imgui specific pipeline for its own functions
	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *pipeline);

	vk::Viewport viewport{};
	viewport.width = drawData->DisplaySize.x;
	viewport.height = drawData->DisplaySize.y;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	commandBuffer.setViewport(0, viewport);

	pushConstBlock.scale = glm::vec2(2.0f / drawData->DisplaySize.x, 2.0f / drawData->DisplaySize.y);
	pushConstBlock.translate = glm::vec2(-1.0f);

	commandBuffer.pushConstants(*pipelineLayout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(PushConstBlock), &pushConstBlock);

	commandBuffer.bindVertexBuffers(0, vertexBuffers, { 0 });
	commandBuffer.bindIndexBuffer(indexBuffers, { 0 }, vk::IndexType::eUint16);

	uint32_t vertexOffset = 0;
	uint32_t indexOffset = 0;

	for (size_t i = 0; i < drawData->CmdListsCount; i++)
	{
		const ImDrawList* cmdList = drawData->CmdLists[i];

		for (size_t j = 0; j < cmdList->CmdBuffer.Size; j++)
		{
			const ImDrawCmd* pcmd = &cmdList->CmdBuffer[j];

			vk::Rect2D scissor{};
			scissor.offset.x = std::max(static_cast<int32_t>(pcmd->ClipRect.x),0);
			scissor.offset.y = std::max(static_cast<int32_t>(pcmd->ClipRect.y), 0);
			scissor.extent.width = static_cast<uint32_t>(pcmd->ClipRect.z - pcmd->ClipRect.x);
			scissor.extent.height = static_cast<uint32_t>(pcmd->ClipRect.w - pcmd->ClipRect.y);

			commandBuffer.setScissor(0, scissor);

			VkDescriptorSet texHandle = (VkDescriptorSet)pcmd->GetTexID();
			if (texHandle)
			{
				commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
					*pipelineLayout, 0, { vk::DescriptorSet(texHandle) }, {});
			}
			else {
				commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
					*pipelineLayout, 0, { *descriptorSet }, {});
			}

			commandBuffer.drawIndexed(pcmd->ElemCount, 1, indexOffset, vertexOffset, 0);
			indexOffset += pcmd->ElemCount;

		}
		vertexOffset += cmdList->VtxBuffer.Size;

	}

	commandBuffer.endRendering();

}


