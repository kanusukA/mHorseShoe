#include "TestingTab.h"



void TestingTabComponent::view()
{
	ImGui::Begin("TESTING");
	ImGui::Text("Cases");
	if (ModelComponent::caseVec)
	{
		for (int i = 0; i < ModelComponent::caseVec->size(); i++)
		{
			ImGui::Text(ModelComponent::caseVec->at(i)->getName().c_str());
		}
	}
	else {
		ImGui::Text("No case made.");
	}
	ImGui::Text("Selected Case");
	if (!ModelComponent::selectedCase->selCase.expired())
	{
		ImGui::Text(ModelComponent::selectedCase->selCase.lock()->getName().c_str());
	}
	else {
		ImGui::Text("None selected");
	}



	ImGui::End();

}
