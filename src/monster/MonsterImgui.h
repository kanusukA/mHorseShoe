#pragma once

#include <monster/MonsterVulkan.h>
#include <monster/imgui-1.92.9b/backends/imgui_impl_sdl3.h>
#include <imgui_stdlib.h>

struct ImguiDebugStats {
	float* mouseXrel;
	float* mouseYrel;

	glm::vec3 position = glm::vec3(0, 0, 0);

};


class MonsterImgui : public MonsterVulkan {
public:

	ImguiDebugStats imDebugStats = ImguiDebugStats();

	bool keyboardCapture = false;

	void InitMonsterImgui();

	void createImguiPipeline();

	void startImguiFrame();
	void renderFrame();


	void ShutdownImgui();

	// debug
	void debugWindow();



};