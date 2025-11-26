#include "Gui.h"

void Gui::_SceneTab() {
	ImGui::Begin("Cases");

	if (ImGui::Button("refresh")) {
		
	}

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