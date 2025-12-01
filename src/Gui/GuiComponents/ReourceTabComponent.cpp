#include "ReourceTabComponent.h"

void ResourceTabComponent::view()
{
	ImGui::Begin("Resources");

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
