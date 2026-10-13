#include <Gui/nGuiComponent/GuiVulkanUtils.h>


void GuiVulkanUtilsView::view()
{
	ImGui::SetNextWindowPos(ImVec2(200, 100));
	ImGui::SetNextWindowSize(ImVec2(200, 300));
	ImGui::Begin("VulkanUtils", 0, ImGuiWindowFlags_::ImGuiWindowFlags_NoMove | ImGuiWindowFlags_::ImGuiWindowFlags_NoResize);

	ImGui::Text("Meshes");

	if (model->renderMeshes && model->renderMeshes->size() > 0)
	{
		for (size_t i = 0; i < model->renderMeshes->size(); i++)
		{
			ImGui::Text("Name : "); ImGui::SameLine();
			ImGui::Text(model->renderMeshes->at(i)->getName().c_str());
			ImGui::Text("Addr : "); ImGui::SameLine();
			ImGui::Text(getAddrChar(model->renderMeshes->at(i)));
			ImGui::Spacing();
		}
	}

	

	

	ImGui::End();
}


