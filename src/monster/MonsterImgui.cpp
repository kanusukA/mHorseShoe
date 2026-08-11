#include <monster/MonsterImgui.h>


void MonsterImgui::InitMonsterImgui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();

	ImGui_ImplSDL3_InitForVulkan(sdlStats->window);

	//vk::PipelineCreateInfoKHR pinfo{};


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
		.DescriptorPoolSize = IMGUI_IMPL_VULKAN_MINIMUM_SAMPLER_POOL_SIZE + IMGUI_IMPL_VULKAN_MINIMUM_SAMPLED_IMAGE_POOL_SIZE ,
		.ImageCount = MAX_FRAMES_IN_FLIGHT,
		.PipelineCache = VK_NULL_HANDLE,
		.PipelineInfoMain = pipelineInfo,

	};

	ImGui_ImplVulkan_Init(&init_info);

	createImguiPipeline();
}


void MonsterImgui::createImguiPipeline()
{
    vk::DescriptorSetLayoutBinding binding;
    binding.descriptorType = vk::DescriptorType::eCombinedImageSampler;
    binding.descriptorCount = 1;
    binding.stageFlags = vk::ShaderStageFlagBits::eFragment;
    binding.binding = 0;

    vk::DescriptorSetLayoutCreateInfo layoutInfo;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &binding;

    vk::raii::DescriptorSetLayout descriptorSetLayout = vk::raii::DescriptorSetLayout(vkMonsterStats.device, layoutInfo);

    vk::DescriptorPoolSize poolSize;
    poolSize.type = vk::DescriptorType::eCombinedImageSampler;
    poolSize.descriptorCount = 1;

    vk::DescriptorPoolCreateInfo poolInfo;
    poolInfo.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
    poolInfo.maxSets = 1;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;

    vk::raii::DescriptorPool descriptorPool = vk::raii::DescriptorPool(vkMonsterStats.device, poolInfo);


    vk::DescriptorSetAllocateInfo allocInfo;
    allocInfo.descriptorPool = *descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &(*descriptorSetLayout);


    vk::raii::DescriptorSets descriptorSets(vkMonsterStats.device, allocInfo);
    vk::raii::DescriptorSet descriptorSet = std::move(descriptorSets[0]); // Store the first (and only) descriptor set
    std::cout << "ImGui created descriptor set with handle: " << *descriptorSet << std::endl;



    vk::PushConstantRange pushConstantRange;
    pushConstantRange.stageFlags = vk::ShaderStageFlagBits::eVertex;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(float) * 4; // 2 floats for scale, 2 floats for translate

    // Create pipeline layout
    vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &(*descriptorSetLayout);
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;


    vk::raii::PipelineLayout pipelineLayout = vk::raii::PipelineLayout(vkMonsterStats.device, pipelineLayoutInfo);

    std::vector<char> shaderCode = std::vector<char>();

    ResourceHandler::GetInstance()->readFileContents("../../../src/monster/shaders/imgui.spv", &shaderCode);

    // Load shaders
    vk::raii::ShaderModule shaderModule = createShaderModule(shaderCode);

    // Shader stage creation
    vk::PipelineShaderStageCreateInfo vertShaderStageInfo;
    vertShaderStageInfo.stage = vk::ShaderStageFlagBits::eVertex;
    vertShaderStageInfo.module = *shaderModule;
    vertShaderStageInfo.pName = "VSMain";

    vk::PipelineShaderStageCreateInfo fragShaderStageInfo;
    fragShaderStageInfo.stage = vk::ShaderStageFlagBits::eFragment;
    fragShaderStageInfo.module = *shaderModule;
    fragShaderStageInfo.pName = "PSMain";

    std::array shaderStages = { vertShaderStageInfo, fragShaderStageInfo };

    // Vertex input
    vk::VertexInputBindingDescription bindingDescription;
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(ImDrawVert);
    bindingDescription.inputRate = vk::VertexInputRate::eVertex;

    std::array<vk::VertexInputAttributeDescription, 3> attributeDescriptions;
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = vk::Format::eR32G32Sfloat;
    attributeDescriptions[0].offset = offsetof(ImDrawVert, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = vk::Format::eR32G32Sfloat;
    attributeDescriptions[1].offset = offsetof(ImDrawVert, uv);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = vk::Format::eR8G8B8A8Unorm;
    attributeDescriptions[2].offset = offsetof(ImDrawVert, col);

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    // Input assembly
    vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
    inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    // Viewport and scissor
    vk::PipelineViewportStateCreateInfo viewportState;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;
    viewportState.pViewports = nullptr; // Dynamic state
    viewportState.pScissors = nullptr; // Dynamic state

    // Rasterization
    vk::PipelineRasterizationStateCreateInfo rasterizer;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = vk::PolygonMode::eFill;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = vk::CullModeFlagBits::eNone;
    rasterizer.frontFace = vk::FrontFace::eCounterClockwise;
    rasterizer.depthBiasEnable = VK_FALSE;

    // Multisampling
    vk::PipelineMultisampleStateCreateInfo multisampling;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;

    // Depth and stencil testing
    vk::PipelineDepthStencilStateCreateInfo depthStencil;
    depthStencil.depthTestEnable = VK_FALSE;
    depthStencil.depthWriteEnable = VK_FALSE;
    depthStencil.depthCompareOp = vk::CompareOp::eLessOrEqual;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;

    // Color blending
    vk::PipelineColorBlendAttachmentState colorBlendAttachment;
    colorBlendAttachment.colorWriteMask =
        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
        vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
    colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
    colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd;
    colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
    colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
    colorBlendAttachment.alphaBlendOp = vk::BlendOp::eAdd;

    vk::PipelineColorBlendStateCreateInfo colorBlending;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;

    // Dynamic state
    std::vector<vk::DynamicState> dynamicStates = {
      vk::DynamicState::eViewport,
      vk::DynamicState::eScissor
    };

    vk::PipelineDynamicStateCreateInfo dynamicState;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    vk::Format depthFormat = findDepthFormat();
    // Create the graphics pipeline with dynamic rendering
    vk::PipelineRenderingCreateInfo renderingInfo;
    renderingInfo.colorAttachmentCount = 1;
    vk::Format colorFormat = vkMonsterStats.swapChainSurfaceFormat.format; // Get the actual swapchain format
    renderingInfo.pColorAttachmentFormats = &colorFormat;
    renderingInfo.depthAttachmentFormat = depthFormat;

    vk::GraphicsPipelineCreateInfo pipelineInfo;
    pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.pNext = &renderingInfo;
    pipelineInfo.basePipelineHandle = nullptr;


    vkMonsterStats.imguiPipeline = std::move(vk::raii::Pipeline(vkMonsterStats.device, nullptr, pipelineInfo));
}


void MonsterImgui::startImguiFrame()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}


void MonsterImgui::renderFrame() {
    ImGui::Render();
    ImDrawData* drawData = ImGui::GetDrawData();
    const bool isMinimized = (drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f);
    if (!isMinimized)
    {
        MonsterVulkan::renderVulkanFrame(drawData);

    }
}

void MonsterImgui::ShutdownImgui()
{
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

void MonsterImgui::debugWindow()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Debug",0,ImGuiWindowFlags_::ImGuiWindowFlags_NoMove);

    ImGui::Text("MouseX : "); ImGui::SameLine();
    if (imDebugStats.mouseXrel)
    {
        ImGui::Text(std::to_string(*imDebugStats.mouseXrel).c_str());
    }
    else {
        ImGui::Text("Value is nullptr");
    }
    ImGui::Text("MouseY : "); ImGui::SameLine();
    if (imDebugStats.mouseYrel)
    {
        ImGui::Text(std::to_string(*imDebugStats.mouseYrel).c_str());
    }
    else {
        ImGui::Text("Value is nullptr");
    }

    ImGui::End();
}
