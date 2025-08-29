#include "Gui.h"

void Gui::_SceneTab() {
	ImGui::Begin("Scenes");

	if (ImGui::Button("Save Scene"))
	{
		SceneHandler::GetInstance()->saveScene("Example");
	}
	ImGui::Spacing();
	if (ImGui::Button("Load Scene"))
	{
		SceneHandler::GetInstance()->loadScenes();
	}

	ImGui::End();
}