#include "TestingTab.h"



void TestingTabComponent::view()
{
	ImGui::Begin("TESTING");

	// MASTER LIST
	if (testModel->masterList)
	{
		for (int i = 0; i < testModel->masterList->size(); i++)
		{
			ImGui::Text((std::to_string(i) + " : " + std::to_string(testModel->masterList->at(i))).c_str());
		}
	}

	ImGui::Spacing();

	// CASE RESOURCE

	ImGui::Text("Cases : ");

	if (testModel->caseResources)
	{
		for (int i = 0; i < testModel->caseResources->size(); i++)
		{
			ImGui::Text(testModel->caseResources->at(i).getName().c_str()); // Why this is coming in as null!! fix this
			ImGui::SameLine();
			ImGui::Text((" : " + std::to_string(testModel->caseResources->at(i).getId())).c_str());
		}
	}

	ImGui::Spacing();

	ImGui::InputText("Case Name",testModel->caseName);
	
	if (ImGui::Button("Add Case"))
	{
		testModel->addCase();
	}

	ImGui::Spacing();
	
	// SCENE RESOURCE

	ImGui::Text("Scenes : ");

	if (testModel->sceneResources)
	{
		for (int i = 0; i < testModel->sceneResources->size(); i++)
		{
			ImGui::Text(testModel->sceneResources->at(i).getName().c_str()); // Why this is coming in as null!! fix this
			ImGui::SameLine();
			ImGui::Text((" : " + std::to_string(testModel->sceneResources->at(i).getId())).c_str());
			ImGui::SameLine();
			
			if (ImGui::SmallButton(("see##" + std::to_string(i)).c_str()))// Fix Id issue
			{
				testModel->getSceneDetails(testModel->sceneResources->at(i).getId());
			}
		}
	}

	ImGui::Spacing();

	ImGui::InputText("Scene Name", &testModel->scnName);

	ImGui::InputFloat3("Position", testModel->scn_pos);

	ImGui::InputFloat4("Orientation", testModel->scn_orientation);

	ImGui::InputFloat3("Scale", testModel->scn_scale);

	ImGui::Spacing();

	ImGui::Text("Scene Type");

	if (ImGui::RadioButton("Static", testModel->scnType == SceneType::STATIC)) {
		testModel->scnType = SceneType::STATIC;
	}

	if (ImGui::RadioButton("Dynamic", testModel->scnType == SceneType::DYNAMIC)) {
		testModel->scnType = SceneType::DYNAMIC;
	}

	if (ImGui::RadioButton("Mesh", testModel->scnType == SceneType::MESH)) {
		testModel->scnType = SceneType::MESH;
	}

	if (ImGui::Button("Add Scene"))
	{
		testModel->addScene();
	}

	ImGui::End();

}
