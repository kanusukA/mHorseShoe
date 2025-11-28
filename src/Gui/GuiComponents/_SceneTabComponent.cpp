#include <Gui/GuiComponents/_SceneTabComponent.h>

void SceneTabComponent::view()
{
	ImGui::Begin("Cases");

	if (ImGui::Button("refresh")) {
		refresh();
	}

	if (ImGui::Button("Save Case"))
	{
		saveCase();
	}
	ImGui::Spacing();


	ImGui::End();
}





// BUTTON FUNCTIONS

void SceneTabComponent::refresh() {
	
	

}

void SceneTabComponent::saveCase() {



}

void SceneTabComponent::loadCase() {



}