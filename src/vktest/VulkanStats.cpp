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



vk::RenderingAttachmentInfo ImGuiVulkanUtil::attachmentInfo(vk::ImageView view, vk::ClearValue* clearView, vk::ImageLayout layout)
{
	vk::RenderingAttachmentInfo attachmentinfo{
		.imageView = view,
		.imageLayout = layout,
		.loadOp = clearView ? vk::AttachmentLoadOp::eClear : vk::AttachmentLoadOp::eLoad,
		.storeOp = vk::AttachmentStoreOp::eStore,
		.clearValue = *clearView
	};
	return attachmentinfo;
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



void ImGuiVulkanUtil::renderImGuiFrame(vk::raii::CommandBuffer& commandBuffer, uint32_t imageIndex)
{
	if (newFrame())
	{
		updateBuffers();
	}
	

	drawFrame(commandBuffer, imageIndex);

	// SUBMITION AND PRESENTATION IS DONE BY MONSTER
}

//void ImGuiVulkanUtil::drawImgui(vk::raii::CommandBuffer& cmdBuffer, vk::ImageView targetView)
//{
//	vk::RenderingAttachmentInfo renderingAttachInfo = attachmentInfo(targetView, nullptr, vk::ImageLayout::eColorAttachmentOptimal);
//
//	vk::RenderingInfo renderingInfo{
//		.colorAttachmentCount = 1,
//		.pColorAttachments = &renderingAttachInfo
//	};
//
//	cmdBuffer.beginRendering(renderingInfo);
//
//	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), *cmdBuffer);
//
//	cmdBuffer.end();
//
//}

//void ImGuiVulkanUtil::immediate_submit()
//{
//	device->resetFences(imguiFence);
//	commandBuffer.reset();
//
//	vk::CommandBufferBeginInfo beginInfo{
//		.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit
//	};
//
//	commandBuffer.begin(beginInfo);
//
//	// ENTER FUNCTION COMMAND HERE
//	commandBuffer.end();
//
//	vk::CommandBufferSubmitInfo cmdSubimtInfo{
//		.commandBuffer = commandBuffer
//	};
//
//	vk::SubmitInfo2 submitInfo{
//		.commandBufferInfoCount = 1,
//		.pCommandBufferInfos = &cmdSubimtInfo
//	};
//
//	graphicsQueue->submit2(submitInfo, imguiFence);
//
//	device->waitForFences(imguiFence, VK_TRUE, UINT64_MAX);
//
//}
//
//void ImGuiVulkanUtil::initCommands()
//{
//	vk::CommandPoolCreateInfo poolInfo{
//		.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
//	};
//
//	commandPool = device->createCommandPool(poolInfo);
//
//	vk::CommandBufferAllocateInfo cmdBufferInfo{
//		.commandPool = commandPool,
//		.level = vk::CommandBufferLevel::ePrimary,
//		.commandBufferCount = 1,
//	};
//
//	commandBuffer = std::move(vk::raii::CommandBuffers(*device,cmdBufferInfo).front());
//
//
//
//}

void ImGuiVulkanUtil::initSync()
{
	vk::FenceCreateInfo fenceInfo{

	};
	imguiFence = device->createFence(fenceInfo);
}



