#include <GuiAddStuff.h>


void GuiAddOverView::view() {
	ImGui::SetNextWindowPos(ImVec2(0, 100));
	ImGui::SetNextWindowSize(ImVec2(150, 400));
	ImGui::Begin("Stack");

	ImGui::Text("Cases"); ImGui::SameLine();
	if (ImGui::Button("Add Case"))
	{
		model->showAddCase = true;
	}

	ImGui::End();
}


void GuiAddCaseView::view() {
	if (model->showAddCase)
	{

		ImGui::Begin("Add New Case");
		ImGui::InputText("Case Name", &caseName);

		ImGui::Spacing();

		ImGui::Begin("Add New Case");
		ImGui::InputText("Case Name", &caseFileName);

		ImGui::Spacing();

		if (ImGui::Button("Create")) {
			model->addCase(caseName, caseFileName);
		}

		ImGui::End();

	}
}