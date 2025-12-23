#include "AddTabComponent.h"

void AddTabComponent::view()
{
	ImGui::Begin("Add");
	ImGui::InputText("Object Name", addModel->objectName);

	// USER CAN SELECT A PREMADE SCENE / CASE(CASE SYSTEM HAS NOT BEEN FULLY IMPLEMENTED)
	//if (addModel->selectedScenes)
	//{
	//	if(addModel->selectedScenes->size() > 0){
	//		if (ImGui::BeginCombo("Scenes", addModel->selectedScenes->at(addModel->selectedScenePos)->getName().c_str()))
	//		{
	//			for (int i = 0; i < addModel->selectedScenes->size(); i++)
	//			{
	//				if (ImGui::Selectable(addModel->selectedScenes->at(i)->getName().c_str(), i == addModel->selectedScenePos))
	//				{
	//					addModel->selectedScenePos = i;
	//				}
	//			}
	//			ImGui::EndCombo();
	//		}
	//	}
	//	else {
	//		ImGui::Text("No Scene Node Found");
	//	}
	//}
	//else
	//{
	//	ImGui::Text("No Scene Node Found");
	//}

	//// OR USER CAN CREATE A SCENE
	//ImGui::Spacing();

	//ImGui::InputText("Scene Name", addModel->CreateSceneNode);



	//if (ImGui::Button("Create Scene"))
	//{
	//	addModel->createSceneBtn();
	//}

	//// USER CAN CHOOSE PHYSX TYPE FOR OBJECT
	//ImGui::Spacing();

	//ImGui::Text("PhysX Type");

	//if (ImGui::RadioButton("DYNAMIC", addModel->SelectedStuffType == StuffType::STUFF_DYNAMIC))
	//{
	//	addModel->selectedStuffTypeDyn();
	//}
	//if (ImGui::RadioButton("STATIC", addModel->SelectedStuffType == StuffType::STUFF_STATIC))
	//{
	//	addModel->selectedStuffTypeStat();
	//}
	//if (ImGui::RadioButton("MESH", addModel->SelectedStuffType == StuffType::STUFF_MESH_ONLY))
	//{
	//	addModel->selectedStuffTypeMesh();
	//}

	//ImGui::Spacing();

	//if (addModel->SelectedStuffType == StuffType::STUFF_DYNAMIC)
	//{
	//	ImGui::InputFloat("Mass", addModel->mass);
	//}

	//// Render Meshes

	//if (addModel->RenderMeshes)
	//{
	//	if (addModel->RenderMeshes.get()->size() > 0)
	//	{
	//		if (ImGui::BeginCombo("Render Mesh", addModel->RenderMeshes.get()->at(addModel->renderMeshesPos).c_str()))
	//		{
	//			for (int i = 0; i < addModel->RenderMeshes.get()->size(); i++)
	//			{
	//				if (ImGui::Selectable(addModel->RenderMeshes.get()->at(i).c_str(), i == addModel->renderMeshesPos))
	//				{
	//					addModel->renderMeshesPos = i;
	//				}
	//			}
	//			ImGui::EndCombo();
	//		}
	//	}
	//	else {
	//		ImGui::Text("No Render Mesh Found.");
	//	}
	//}
	//else {
	//	ImGui::Text("No Render Mesh Found.");
	//}

	//// Render Meshes

	//if (addModel->ColliderMeshes)
	//{
	//	if (addModel->ColliderMeshes->size() > 0)
	//	{
	//		if (ImGui::BeginCombo("Collider Meshes", addModel->ColliderMeshes->at(addModel->colliderMeshesPos).filename().string().c_str()))
	//		{
	//			for (int i = 0; i < addModel->ColliderMeshes->size(); i++)
	//			{
	//				if (ImGui::Selectable(addModel->ColliderMeshes->at(i).filename().string().c_str(), i == addModel->colliderMeshesPos))
	//				{
	//					addModel->colliderMeshesPos = i;
	//				}
	//			}
	//			ImGui::EndCombo();
	//		}
	//	}
	//	else
	//	{
	//		ImGui::Text("No Collider Mesh Found.");
	//	}
	//}
	//else {
	//	ImGui::Text("No Collider Mesh Found."); // Generally should not happen. Resouurce handler adds an box name by default.
	//}

	//if (addModel->colliderMeshesPos == 0)
	//{
	//	ImGui::InputFloat3("Collider Size", addModel->colSize);
	//}

	//ImGui::Spacing();

	//ImGui::InputInt3("Position", addModel->pos);

	//if (ImGui::InputFloat4("Rotation", addModel->rot))
	//{
	//	addModel->normalizeRot();
	//}

	//if (ImGui::Button("Add"))
	//{
	//	addModel->add();
	//}

	ImGui::End();

}
