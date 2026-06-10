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

	ImGui::Text("Default Case : ");
	if (!model->defaultCase.expired())
	{
		ImGui::SameLine();
		ImGui::Text(model->defaultCase.lock().get()->c_str());
	}

	if (ImGui::Button("Refresh"))
	{
		model->refresh();
	}
	
	ImVariableText("Case", mFontType::TTITLE32);
	ImGui::BeginChild("Load / Save", ImVec2(350, 100));
	

	if (ModelComponent::savedCaseFiles && !ModelComponent::savedCaseFiles->empty())
	{
		if (ImGui::BeginCombo("Saved Cases", ModelComponent::savedCaseFiles->at(model->selectedSavedCaseFile).filename().string().c_str()))
		{
			for (int i = 0; i < ModelComponent::savedCaseFiles->size(); i++)
			{
				if (ImGui::Selectable(ModelComponent::savedCaseFiles->at(i).filename().string().c_str(), model->selectedSavedCaseFile == i))
				{
					model->selectedSavedCaseFile = i;
				}
			}
			ImGui::EndCombo();
		}
	}

	if (ImGui::Button("Save Case"))
	{
		model->saveCase();
	}
	ImGui::SameLine();
	if (ImGui::Button("Load Case"))
	{
		model->loadCase();
	}
	if (ImGui::Button("Set Default"))
	{
		model->saveDefaultCase();
		// make set default text
	}
	
	ImGui::EndChild();

	ImGui::Separator();

	ImGui::InputText("Case Name", model->inputCaseName);
	if (ImGui::Button("Create Case"))
	{
		model->addCase();
	}

	//ImGui::Text("Current Case : ");
	if (!CaseHandler::selectedCase)
	{
		ImGui::Text("No Case Selected");
	}
	else {
		

		//ImGui::SameLine();
		//ImGui::Text((ModelComponent::selectedCase->selCase.lock()->getName() + "##Selected_Case").c_str());

		ImGui::Text("Scenes");

		// THIS WAS NULL
		scenesView(CaseHandler::selectedCase->getScenes(), model);

		ImGui::Spacing(); ImGui::Spacing();

		ImGui::SetNextItemWidth(180);
		ImGui::InputText("Scene Name", model->inputSceneName);

		// SCENE TYPE SELECTION
		if (ImGui::RadioButton("Static", model->scnType == SceneType::STATIC))
		{
			model->scnType = SceneType::STATIC;
		}
		ImGui::SameLine();

		if (ImGui::RadioButton("Dynamic", model->scnType == SceneType::DYNAMIC)) {
			model->scnType = SceneType::DYNAMIC;
		}
		ImGui::SameLine();

		if (ImGui::RadioButton("Mesh", model->scnType == SceneType::MESH)) {
			model->scnType = SceneType::MESH;
		}

		if (ImGui::Button("Add Scene"))
		{
			model->addScene();
		}

		ImGui::Spacing(); ImGui::Spacing();


		//ImGui::Text("Selected Scene : ");
		if (!model->selectedScene.expired() && !model->isRootScnNodeSelected())
		{
			//ImGui::SameLine();
			//ImGui::Text(ModelComponent::selectedScene->selScene.lock()->getName().c_str());

			if (ImGui::Button("Select Root"))
			{
				model->selectRootSceneNode();
			}

			ImGui::Spacing();

			// ADD SCENE


			ImGui::Text("SCENE : ");
			ImGui::SameLine();
			if (!model->selectedScene.expired()) // SCENE IS SELECTED
			{
				// Scene Name
				//ImGui::Text(model->selectedScene.lock()->getName().c_str());

				ImGui::Text("TYPE : "); ImGui::SameLine();
				switch (model->selectedScene.lock()->getSceneType())
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
				if (ImGui::InputFloat3("Position", model->selectedScene.lock()->_getPosition())) {
					// _setPosition function ,create it!!
					model->selectedScene.lock()->updatePosition();
				}
				if (ImGui::InputFloat4("Rotation", model->selectedScene.lock()->_getOrientation())) {
					model->selectedScene.lock()->updateOrientation();
				}
				if (ImGui::InputFloat3("Scale", model->selectedScene.lock()->_getScale())) {
					model->selectedScene.lock()->updateScale();
				}

				ImGui::Spacing(); ImGui::Spacing();

				// ADD OBJECT
				ImGui::Text("Add Object : ");
				if (ModelComponent::meshDpVec && ModelComponent::meshDpVec->size() > 0) {

					ImGui::InputText("Object Name", model->inputObjectname);

					if (ImGui::BeginCombo("Render Meshes", ModelComponent::meshDpVec->at(model->selectedMesh).filename().string().c_str()))
					{
						for (int i = 0; i < ModelComponent::meshDpVec->size(); i++)
						{
							if (ImGui::Selectable(ModelComponent::meshDpVec->at(i).filename().string().c_str(), model->selectedMesh == i))
							{
								model->selectedMesh = i;
							}
						}

						ImGui::EndCombo();
					}

					if (ImGui::Button("Add Object"))
					{
						// TODO ADD OBJECT
						model->addObject();
					}

				}
				else
				{
					ImGui::Text("No Render Mesh available");
				}

				// Objects
				if (model->selectedScene.lock()->getObjects())
				{
					for (int i = 0; i < model->selectedScene.lock()->getObjects()->size(); i++)
					{
						ImGui::SetNextItemWidth(180);
						ImGui::Text(model->selectedScene.lock()->getObjects()->at(i)->getName().c_str());
						ImGui::SameLine(180);
						if (ImGui::Button(("Select##" + std::to_string(i)).c_str()))
						{
							model->selectObject(model->selectedScene.lock()->getObjects()->at(i));
						}
						ImGui::SameLine();
						if (ImGui::Button(("Delete##" + std::to_string(i)).c_str()))
						{
							model->deleteObject(i);

						}
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

