#include <Gui/mediator/GuiMediator.h>

void SceneTabComponent::View() {

	ImGui::Begin("Cases");

	if (ImGui::Button("refresh")) {
		refresh();
	}

	if (ImGui::Button("Save Case"))
	{
		saveCase();
		SceneHandler::GetInstance()->saveScene("Example");
	}
	ImGui::Spacing();
	
	//NULL CHECK CASES
	if (this->CasesNames)
	{
		for (int i = 0; i < CasesNames->size(); i++)
		{
			ImGui::Text(CasesNames->at(i).c_str());
		}
	}
	else {
		ImGui::Text("No Case Found!");
	}

	ImGui::End();

}

void SceneTabComponent::refresh() {
	
}