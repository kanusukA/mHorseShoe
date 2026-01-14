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

	// SCENE TYPE SELECTION
	if (ImGui::RadioButton("Static", scnTabModel->scnType == SceneType::STATIC) )
	{
		scnTabModel->scnType = SceneType::STATIC;
	}
	ImGui::SameLine();

	if (ImGui::RadioButton("Dynamic", scnTabModel->scnType == SceneType::DYNAMIC)) {
		scnTabModel->scnType = SceneType::DYNAMIC;
	}
	ImGui::SameLine();

	if (ImGui::RadioButton("Mesh", scnTabModel->scnType == SceneType::MESH)) {
		scnTabModel->scnType = SceneType::MESH;
	}

	if (ImGui::Button("Add Scene"))
	{
		scnTabModel->addScene();
	}

	ImGui::Spacing(); ImGui::Spacing();

	ImGui::Text("Scenes");

	scenesView(scnTabModel->currentCase->getScenes(),scnTabModel);

	ImGui::Spacing(); ImGui::Spacing();

	
	// ADD SCENE OBJECTS CREATIONS AND OBJECTS VIEW!!

	
	ImGui::Text("SCENE : ");
	ImGui::SameLine();
	if (scnTabModel->currentCase->getSelectedScene()) // SCENE IS SELECTED
	{
		// Scene Name
		ImGui::Text(scnTabModel->currentCase->getSelectedScene()->getName().c_str());

		ImGui::Text("TYPE : "); ImGui::SameLine();
		switch (scnTabModel->currentCase->getSelectedScene()->getSceneType())
		{
		case SceneType::STATIC:
			ImGui::Text("STATIC");
			break;
		case SceneType::DYNAMIC:
			ImGui::Text("DYNAMIC");
			break;
		case SceneType::MESH:
			ImGui::Text("MESH");
			break;
		default:
			break;
		}

		// Scene position , rotation, scale
		if (ImGui::InputFloat3("Position", scnTabModel->currentCase->getSelectedScene()->_getPosition())) {
			// _setPosition function ,create it!!
			scnTabModel->currentCase->getSelectedScene()->updatePosition();
		}
		if (ImGui::InputFloat4("Rotation", scnTabModel->currentCase->getSelectedScene()->_getOrientation())) {
			scnTabModel->currentCase->getSelectedScene()->updateOrientation();
		}
		if (ImGui::InputFloat3("Scale", scnTabModel->currentCase->getSelectedScene()->_getScale())) {
			scnTabModel->currentCase->getSelectedScene()->updateScale();
		}

		ImGui::Spacing(); ImGui::Spacing();

		// ADD OBJECT
		ImGui::Text("Add Object : ");
		if(scnTabModel->renderMeshes){

			ImGui::InputText("Object Name", scnTabModel->inputObjectname);

			if (ImGui::BeginCombo("Render Meshes", scnTabModel->renderMeshes->at(scnTabModel->selectedMesh).filename().string().c_str()))
			{
				for (int i = 0; i < scnTabModel->renderMeshes->size(); i++)
				{
					if (ImGui::Selectable(scnTabModel->renderMeshes->at(i).filename().string().c_str(), scnTabModel->selectedMesh == i))
					{
						scnTabModel->selectedMesh = i;
					}
				}

				ImGui::EndCombo();
			}

			if (ImGui::Button("Add Object"))
			{
				// TODO ADD OBJECT
				scnTabModel->addObject();
			}

		}
		else
		{
			ImGui::Text("No Render Mesh available");
		}

		// Objects
		if (scnTabModel->currentCase->getSelectedScene()->getObjects())
		{
			for (int i = 0; i < scnTabModel->currentCase->getSelectedScene()->getObjects()->size(); i++)
			{
				ImGui::SetNextItemWidth(180);
				ImGui::Text(scnTabModel->currentCase->getSelectedScene()->getObjects()->at(i)->getName().c_str());
				ImGui::SameLine(180);
				if (ImGui::Button(("Select##" + std::to_string(i)).c_str()))
				{
					scnTabModel->selectObject(scnTabModel->currentCase->getSelectedScene()->getObjects()->at(i));

				}
				ImGui::SameLine();
				if (ImGui::Button(("Delete##" + std::to_string(i)).c_str()))
				{
					scnTabModel->deleteObject(scnTabModel->currentCase->getSelectedScene()->getObjects()->at(i)->getId());

				}
			}
		}

	}
	else { // NO SCENE IS SELECTED
		ImGui::Text("No Scene Selected");
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