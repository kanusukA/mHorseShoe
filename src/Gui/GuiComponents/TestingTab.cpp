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

	if (testModel->caseResources)
	{
		for (int i = 0; i < testModel->caseResources->size(); i++)
		{
			ImGui::Text(testModel->caseResources->at(i)->getName());/// Why this is coming in as null!! fix this
			ImGui::SameLine();
			ImGui::Text((" : " + std::to_string(testModel->caseResources->at(i)->getId())).c_str());
		}
	}

	ImGui::Spacing();

	ImGui::InputText("Case Name",testModel->caseName);
	
	if (ImGui::Button("Add Case"))
	{
		testModel->addCase();
	}


	ImGui::End();

}
