#include "ReourceTabComponent.h"


void pathViewComponent(
	ResourceTabModelComponent* model
) {

	if (ImGui::CollapsingHeader("Paths")) {

		

		for (int i = 0; i < model->paths->size(); i++)
		{
			if (model->edit == i)
			{
				if (ImGui::InputText("", model->inputPath, ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue)) {
					model->setPath();
				}
				ImGui::SameLine();
				if (ImGui::Button("Set"))
				{
					model->setPath();
				}
			}
			else {
				ImGui::Text(model->paths->at(i).c_str(), 500.0);
				ImGui::SameLine();
				if (ImGui::Button(("Edit##" + std::to_string(i)).c_str()))
				{
					model->editPath(i);
				}
			}

			
		}
	}

}

void ResourceTabComponent::view()
{
	ImGui::Begin("Resources");

	pathViewComponent(resourceTabModel);

	ImGui::Columns(2);

	if (ImGui::Button("Fetch All Resources"))
	{
		resourceTabModel->FetchAllResourcesBtn();
	}

	ImGui::Spacing();

	ImGui::Text("RENDER MESHES : ");

	ImGui::Spacing();

	// renderMeshes
	if (resourceTabModel->renderMeshes)
	{
		for (int i = 0; i < resourceTabModel->renderMeshes->size(); i++)
		{
			ImGui::Text(resourceTabModel->renderMeshes->at(i).filename().string().c_str());
		}
	}
	else {
		ImGui::Text("No Render Mesh Found!");
	}

	ImGui::Spacing();

	// Collider Meshes
	if (resourceTabModel->colliderMeshes)
	{
		for (int i = 0; i < resourceTabModel->colliderMeshes->size(); i++)
		{
			ImGui::Text(resourceTabModel->colliderMeshes->at(i).filename().string().c_str());
		}
	}
	else {
		ImGui::Text("No Collider Mesh Found!");
	}

	ImGui::Spacing();

	// Materials
	if (resourceTabModel->meshMaterials)
	{
		for (int i = 0; i < resourceTabModel->meshMaterials->size(); i++)
		{
			ImGui::Text(resourceTabModel->meshMaterials->at(i).filename().string().c_str());
		}
	}
	else {
		ImGui::Text("No Material Found!");
	}

	ImGui::Spacing();

	ImGui::NextColumn();

	if (ImGui::Button("Render Mesh To Ogre"))
	{
		resourceTabModel->RenderMeshToOgreBtn();
	}

	ImGui::Spacing();

	// Ogre meshes
	if (resourceTabModel->ogreMeshes)
	{
		for (int i = 0; i < resourceTabModel->ogreMeshes->size(); i++)
		{
			ImGui::Text(resourceTabModel->ogreMeshes->at(i).c_str());
		}
	}
	else {
		ImGui::Text("No Ogre Mesh Found!");
	}

	ImGui::End();
}
