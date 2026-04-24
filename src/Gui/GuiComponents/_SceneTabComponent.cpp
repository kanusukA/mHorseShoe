#include <Gui/GuiComponents/_SceneTabComponent.h>

inline void scenesView(const std::vector<std::shared_ptr<Scene>>* scene, SceneTabModelComponent* model,int recurPos = 0) {
	
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
				model->deleteScene(i);
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

	if (ModelComponent::savedCaseFiles && !ModelComponent::savedCaseFiles->empty())
	{
		if (ImGui::BeginCombo("Saved Cases", ModelComponent::savedCaseFiles->at(scnTabModel->selectedSavedCaseFile).filename().string().c_str()))
		{
			for (int i = 0; i < ModelComponent::savedCaseFiles->size(); i++)
			{
				if (ImGui::Selectable(ModelComponent::savedCaseFiles->at(i).filename().string().c_str(), scnTabModel->selectedSavedCaseFile == i))
				{
					scnTabModel->selectedSavedCaseFile = i;
				}
			}
			ImGui::EndCombo();
		}
	}

	if (ModelComponent::caseVec && !ModelComponent::caseVec->empty())
	{
		if (ImGui::BeginCombo("Cases",ModelComponent::caseVec->at(scnTabModel->selectedCase)->getName().c_str()))
		{
			for (int i = 0; i < ModelComponent::caseVec->size(); i++)
			{
				if (ImGui::Selectable((ModelComponent::caseVec->at(i)->getName() + "##Cases_" + std::to_string(i)).c_str(), scnTabModel->selectedCase == i))
				{
					scnTabModel->selectedCase = i;
					scnTabModel->selectCase(i);
				}
			}
			ImGui::EndCombo();
		}
	}

	if (ImGui::Button("Save Case"))
	{
		scnTabModel->saveCase();
	}
	ImGui::SameLine();
	if (ImGui::Button("Load Case"))
	{
		scnTabModel->loadCase();
	}

	ImGui::InputText("Case Name", scnTabModel->inputCaseName);
	if (ImGui::Button("Create Case"))
	{
		scnTabModel->addCase();
	}

	ImGui::Text("Current Case : ");
	if (ModelComponent::selectedCase->selCase.expired())
	{
		ImGui::Text("No Case Selected");
	}
	else {
		

		ImGui::SameLine();
		ImGui::Text((ModelComponent::selectedCase->selCase.lock()->getName() + "##Selected_Case").c_str());


		ImGui::Text("Selected Scene : ");
		if (!ModelComponent::selectedScene->selScene.expired() && !scnTabModel->isRootScnNodeSelected())
		{
			ImGui::SameLine();
			ImGui::Text(ModelComponent::selectedScene->selScene.lock()->getName().c_str());
		}

		if (ImGui::Button("Select Root"))
		{
			scnTabModel->selectRootSceneNode();
		}


		ImGui::Spacing();

		// ADD SCENE

		ImGui::Text("Scene Name : ");
		ImGui::SetNextItemWidth(180);
		ImGui::InputText("Scene Name", scnTabModel->inputSceneName);

		// SCENE TYPE SELECTION
		if (ImGui::RadioButton("Static", scnTabModel->scnType == SceneType::STATIC))
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

		scenesView(ModelComponent::selectedCase->selCase.lock()->getScenes(), scnTabModel);

		ImGui::Spacing(); ImGui::Spacing();


		// ADD SCENE OBJECTS CREATIONS AND OBJECTS VIEW!!


		ImGui::Text("SCENE : ");
		ImGui::SameLine();
		if (!ModelComponent::selectedScene->selScene.expired()) // SCENE IS SELECTED
		{
			// Scene Name
			ImGui::Text(ModelComponent::selectedScene->selScene.lock()->getName().c_str());

			ImGui::Text("TYPE : "); ImGui::SameLine();
			switch (ModelComponent::selectedScene->selScene.lock()->getSceneType())
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
			if (ImGui::InputFloat3("Position", ModelComponent::selectedScene->selScene.lock()->_getPosition())) {
				// _setPosition function ,create it!!
				ModelComponent::selectedScene->selScene.lock()->updatePosition();
			}
			if (ImGui::InputFloat4("Rotation", ModelComponent::selectedScene->selScene.lock()->_getOrientation())) {
				ModelComponent::selectedScene->selScene.lock()->updateOrientation();
			}
			if (ImGui::InputFloat3("Scale", ModelComponent::selectedScene->selScene.lock()->_getScale())) {
				ModelComponent::selectedScene->selScene.lock()->updateScale();
			}

			ImGui::Spacing(); ImGui::Spacing();

			// ADD OBJECT
			ImGui::Text("Add Object : ");
			if (ModelComponent::meshDpVec && ModelComponent::meshDpVec->size() > 0) {

				ImGui::InputText("Object Name", scnTabModel->inputObjectname);

				if (ImGui::BeginCombo("Render Meshes", ModelComponent::meshDpVec->at(scnTabModel->selectedMesh).filename().string().c_str()))
				{
					for (int i = 0; i < ModelComponent::meshDpVec->size(); i++)
					{
						if (ImGui::Selectable(ModelComponent::meshDpVec->at(i).filename().string().c_str(), scnTabModel->selectedMesh == i))
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
			if (ModelComponent::selectedScene->selScene.lock()->getObjects())
			{
				for (int i = 0; i < ModelComponent::selectedScene->selScene.lock()->getObjects()->size(); i++)
				{
					ImGui::SetNextItemWidth(180);
					ImGui::Text(ModelComponent::selectedScene->selScene.lock()->getObjects()->at(i)->getName().c_str());
					ImGui::SameLine(180);
					if (ImGui::Button(("Select##" + std::to_string(i)).c_str()))
					{
						scnTabModel->selectObject(ModelComponent::selectedScene->selScene.lock()->getObjects()->at(i));

					}
					ImGui::SameLine();
					if (ImGui::Button(("Delete##" + std::to_string(i)).c_str()))
					{
						scnTabModel->deleteObject(i);

					}
				}
			}

		}
		else { // NO SCENE IS SELECTED
			ImGui::Text("No Scene Selected");
		}




		ImGui::Spacing();

	}

	ImGui::End();
}

