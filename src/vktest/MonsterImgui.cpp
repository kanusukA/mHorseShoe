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
		.DescriptorPoolSize = 1,
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
		renderVulkanFrame(drawData);

	}
}

void Monster::ShutdownImGui()
{
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
}
