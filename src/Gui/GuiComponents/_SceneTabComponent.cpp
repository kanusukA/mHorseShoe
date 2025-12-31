#include <Gui/GuiComponents/_SceneTabComponent.h>

void scenesView(std::vector<Scene*>* scene, SceneTabModelComponent* model,int recurPos = 0) {
	
	for (int i = 0; i < scene->size(); i++)
	{
		if (scene->at(i))
		{
			ImGui::Indent(2 * recurPos);

			ImGui::SetNextItemWidth(150);
			ImGui::Text(scene->at(i)->getName().c_str());
			ImGui::SameLine(150);


			if (ImGui::Button(("Select##" + scene->at(i)->getName()).c_str()))
			{
				model->selectScene(scene->at(i));

			}
			ImGui::SameLine();
			if (ImGui::Button(("Delete##" + scene->at(i)->getName()).c_str()))
			{
				model->deleteScene(scene->at(i));
				break;

			}

			if (scene->at(i)->getAttachedScenes()) {
				if (!scene->at(i)->getAttachedScenes()->empty())
				{
					scenesView(scene->at(i)->getAttachedScenes(), model, recurPos + 1);
				}
			}

			ImGui::Unindent(5 * recurPos);
		}
		
		
	}
	
}

void SceneTabComponent::view()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(350, 600));
	ImGui::Begin("Cases",0,ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	ImGui::Text("Current Case : ");
	ImGui::SameLine();
	ImGui::Text(this->scnTabModel->currentCase->getName().c_str());

	ImGui::Text("Selected Scene : ");
	if (scnTabModel->currentCase->getSelectedScene() && !scnTabModel->isRootScnNodeSelected())
	{
		ImGui::SameLine();
		ImGui::Text(scnTabModel->currentCase->getSelectedScene()->getName().c_str());
	}

	if (ImGui::Button("Select Root"))
	{
		scnTabModel->selectRootSceneNode();
	}
	

	ImGui::Spacing();

	// ADD SCENE
	
	ImGui::Text("Scene Name : ");
	ImGui::SetNextItemWidth(180);
	ImGui::InputText("", scnTabModel->inputSceneName);

	if (ImGui::Button("Add Scene"))
	{
		scnTabModel->addScene();
	}

	ImGui::Spacing();

	ImGui::Text("Scenes");

	scenesView(scnTabModel->currentCase->getScenes(),scnTabModel);

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