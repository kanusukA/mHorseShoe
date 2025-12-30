#include <Gui/GuiComponents/_SceneTabComponent.h>

void scenesView(std::vector<Scene*>* scene,int recurPos = 0) {
	for (int i = 0; i < scene->size(); i++)
	{
		ImGui::Text(scene->at(i)->getName().c_str());
		if(scene->at(i)->getAttachedScenes()){
			if (!scene->at(i)->getAttachedScenes()->empty())
			{
				scenesView(scene->at(i)->getAttachedScenes(), recurPos += 1);
			}
		}
	}
}

void SceneTabComponent::view()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(250, 500));
	ImGui::Begin("Cases",0,ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	ImGui::Text("Current Case : ");
	ImGui::SameLine();
	ImGui::Text(this->scnTabModel->currentCase->getName().c_str());

	ImGui::Spacing();

	// ADD SCENE
	
	ImGui::Text("Scene Name : ");
	ImGui::InputText("", scnTabModel->inputSceneName);

	if (ImGui::Button("Add Scene"))
	{
		scnTabModel->addScene();
	}

	ImGui::Spacing();

	ImGui::Text("Scenes");

	scenesView(scnTabModel->currentCase->getScenes());

	/*if (ImGui::BeginTable("scene_table", 3)) {
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
	}*/
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