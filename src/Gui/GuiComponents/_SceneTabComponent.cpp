#include <Gui/GuiComponents/_SceneTabComponent.h>

void SceneTabComponent::view()
{
	ImGui::Begin("Cases");

	if (scnTabModel->cases)
	{
		for (int i = 0; i < scnTabModel->cases->size(); i++)
		{
			ImGui::Text(scnTabModel->cases->at(i).c_str());
		}
	}

	ImGui::Spacing();

	if (ImGui::Button("refresh")) {
		scnTabModel->refresh();
	}

	if (ImGui::Button("Save Case"))
	{
		scnTabModel->saveCase();
	}
	ImGui::Spacing();


	ImGui::End();
}





// BUTTON FUNCTIONS
void SceneTabModelComponent::refresh() {
	this->gdSource->getSceneHandler()->loadCases();
}

void SceneTabModelComponent::saveCase() {

}

void SceneTabModelComponent::loadCase() {


}