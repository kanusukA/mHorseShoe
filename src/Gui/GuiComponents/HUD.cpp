#pragma once

#include <base/Mediator.h>
#include <OgreImGuiOverlay.h>

class HUDComponent : public ViewComponent
{
public:
	HUDComponent(const char* name_p) : ViewComponent(name_p) {

	}

	void view() override {

		ImDrawList* draw = ImGui::GetBackgroundDrawList();

		draw->AddLine(ImVec2(ImGui::GetMainViewport()->GetCenter().x, ImGui::GetMainViewport()->GetCenter().y - 50),
			ImVec2(ImGui::GetMainViewport()->GetCenter().x, ImGui::GetMainViewport()->GetCenter().y + 50), ImColor(120, 50, 120));

		draw->AddLine(ImVec2(ImGui::GetMainViewport()->GetCenter().x - 50, ImGui::GetMainViewport()->GetCenter().y),
			ImVec2(ImGui::GetMainViewport()->GetCenter().x + 50, ImGui::GetMainViewport()->GetCenter().y), ImColor(120, 50, 120));
	}
};