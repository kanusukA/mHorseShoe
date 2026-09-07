#include <Gui/nGuiComponent/GuiGenStats.h>

void GuiGenStatsView::view()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(200, 100));
	ImGui::Begin("DEBUG STATS");

	if (!model->debugStats)
	{
		ImGui::Text("DEBUG STATS NOT SET!");
		ImGui::End(); 
		return;
	}

	ImGui::Text("Mouse X : "); ImGui::SameLine();
	TextFloatP(model->debugStats->mouseXrel);

	ImGui::Text("Mouse Y : "); ImGui::SameLine();
	TextFloatP(model->debugStats->mouseYrel);

	ImGui::End();
}
