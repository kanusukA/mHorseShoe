#include <Gui/nGuiComponent/GuiAddStuff.h>


void SceneViewer(const std::vector<std::shared_ptr<Scene>>* scenes,GuiAddStuffModel* model) { // ADD INDEXING
	
		for (size_t i = 0; i < scenes->size(); i++)
		{
			if (ImGui::TreeNode(scenes->at(i)->getName().c_str()))
			{
				ImGui::SameLine();
				if (ImGui::Button("Add Scene##"))
				{
					model->selectedScene = scenes->at(i);
					model->showAddSceneToScene = true;
				}
				SceneViewer(scenes->at(i)->getAttachedScenes(), model);

				ImGui::TreePop();
			}
			
		}
	
	
}


void GuiAddOverView::view() {
	ImGui::SetNextWindowPos(ImVec2(0, 100));
	ImGui::SetNextWindowSize(ImVec2(300, 500));
	ImGui::Begin("Stack");

	ImGui::Text("Cases"); ImGui::SameLine();
	if (ImGui::Button("Add Case"))
	{
		model->showAddCase = true;
	}

	if (model->caseHandler && model->caseHandler->mCases)
	{
		for (size_t i = 0; i < model->caseHandler->mCases->size(); i++)
		{
			//ImGui::Text(model->caseHandler->mCases->at(i)->getName().c_str()); ImGui::SameLine();
			
			if (ImGui::TreeNode(model->caseHandler->mCases->at(i)->getName().c_str()))
			{
				if (ImGui::Button("Add Scene"))
				{
					model->selectedCase = model->caseHandler->mCases->at(i);
					model->showAddSceneToCase = true;
				}

				SceneViewer(model->caseHandler->mCases->at(i)->getScenes(), model);

				ImGui::TreePop();
			}
			
		}
	}

	ImGui::End();
}


void GuiAddCaseView::view() {
	if (model->showAddCase)
	{

		ImGui::Begin("Add New Case");
		ImGui::InputText("Case Name", &caseName);

		ImGui::Spacing();

		ImGui::InputText("Case File Name", &caseFileName);

		ImGui::Spacing();

		if (ImGui::Button("Create Case")) {
			model->addCase(caseName, caseFileName);
		}

		ImGui::End();

	}
}

void GuiAddSceneView::view()
{
	if (model->showAddSceneToCase)
	{
		ImGui::Begin("Add New Scene");
		ImGui::Text(("Attach To Case: " + model->selectedCase->getName()).c_str());
		ImGui::InputText("Scene Name", &sceneName);
		ImGui::Spacing();

		if (ImGui::Button("Create Scene"))
		{
			model->addScene(model->selectedCase,sceneName);
			sceneName = "";
		}

		ImGui::End();
	}

	if (model->showAddSceneToScene)
	{
		ImGui::Begin("Add New Scene");
		ImGui::Text(("Attach To Scene: " + model->selectedScene->getName()).c_str());
		ImGui::InputText("Scene Name", &sceneName);
		ImGui::Spacing();

		if (ImGui::Button("Create Scene"))
		{
			model->addSceneToScene(model->selectedScene, sceneName);
			sceneName = "";
		}

		ImGui::End();
	}
}
