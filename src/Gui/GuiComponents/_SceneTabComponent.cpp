#include <Gui/GuiComponents/_SceneTabComponent.h>

void SceneTabComponent::view()
{
	ImGui::Begin("Cases");

	ImGui::Text("Current Case : ");
	ImGui::SameLine();
	ImGui::Text(this->scnTabModel->currentCase->getName().c_str());

	ImGui::Spacing();

	ImGui::Text("Scenes");

	if (ImGui::BeginTable("scene_table", 3)) {
		for (int i = 0; i < scnTabModel->currentCase->getSceneCount(); i++)
		{
			SceneResource* scnResource = scnTabModel->getCaseScene(scnTabModel->currentCase->getScenesIdInCase()->at(i));
			switch (scnResource->getSceneType())
			{
			case SceneType::STATIC :
				ImGui::TableSetColumnIndex(0);
				break;
			case SceneType::DYNAMIC :
				ImGui::TableSetColumnIndex(1);
				break;
			case SceneType::MESH :
				ImGui::TableSetColumnIndex(2);
				break;
			default:
				break;
			}

			ImGui::Text(scnResource->getName().c_str());

			ImGui::TableNextRow();
		
		}ImGui::EndTable();
	}



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
	//TODO Implement button after creating resource Handler Loader class
}

void SceneTabModelComponent::saveCase() {

}

void SceneTabModelComponent::loadCase() {


}