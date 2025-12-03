#include "StatusTabComponent.h"

void StatusTabComponent::view()
{
	ImGui::SetNextWindowSize(ImVec2(250, 100));
	ImGui::SetNextWindowPos(ImVec2(0, 0));

	ImGui::Begin("Status", 0, ImGuiWindowFlags_NoMove && ImGuiWindowFlags_NoCollapse && ImGuiWindowFlags_NoResize);

	if (this->statusModel->isPlayerObserverSet())
	{
		ImGui::Text("Position");
		ImGui::Value("X", this->statusModel->getPlayerPosition().x);
		ImGui::SameLine();
		ImGui::Value("Y", this->statusModel->getPlayerPosition().y);
		ImGui::SameLine();
		ImGui::Value("Z", this->statusModel->getPlayerPosition().z);

		ImGui::Text("Rotation");
		ImGui::Value("W", this->statusModel->getPlayerOrientation().w);
		ImGui::SameLine();
		ImGui::Value("X", this->statusModel->getPlayerOrientation().x);
		ImGui::SameLine();
		ImGui::Value("Y", this->statusModel->getPlayerPosition().y);
		ImGui::SameLine();
		ImGui::Value("Z", this->statusModel->getPlayerPosition().z);
	}
	else {
		ImGui::Text("No Player Observer Set");
	}
	

	ImGui::End();

}
