
#include <monster/MonsterVulkan.h>
#include <imgui.h>
#include <monster/imgui-1.92.9b/backends/imgui_impl_sdl3.h>
#include <monster/imgui-1.92.9b/backends/imgui_impl_vulkan.h>


class MonsterImgui : public MonsterVulkan {
public:

	void InitMonsterImgui();

	void createImguiPipeline();

	void startImguiFrame();
	void renderFrame();


	void ShutdownImgui();



};