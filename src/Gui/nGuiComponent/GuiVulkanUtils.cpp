#include <Gui/nGuiComponent/GuiVulkanUtils.h>

void GuiVulkanUtilsView::view()
{
	ImGui::SetNextWindowPos(ImVec2(0, 100));
	ImGui::SetNextWindowSize(ImVec2(200, 300));
	ImGui::Begin("VulkanUtils", 0, ImGuiWindowFlags_::ImGuiWindowFlags_NoMove | ImGuiWindowFlags_::ImGuiWindowFlags_NoResize);

	ImGui::Text("Meshes");

	

	ImGui::End();
}
