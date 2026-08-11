
#include <monster/MonsterVulkan.h>
#include <imgui.h>
#include <monster/imgui-1.92.9b/backends/imgui_impl_sdl3.h>
#include <monster/imgui-1.92.9b/backends/imgui_impl_vulkan.h>

struct ImguiDebugStats {
	float* mouseXrel;
	float* mouseYrel;

	glm::vec3 position = glm::vec3(0, 0, 0);

};


class MonsterImgui : public MonsterVulkan {
public:

	ImguiDebugStats imDebugStats = ImguiDebugStats();

	void InitMonsterImgui();

	void createImguiPipeline();

	void startImguiFrame();
	void renderFrame();


	void ShutdownImgui();

	// debug
	void debugWindow();



};