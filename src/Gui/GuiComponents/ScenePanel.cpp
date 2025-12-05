#include "ScenePanel.h"

void ScenePanelTabComponent::view()
{
	ImGui::Begin("Stuffs");

	ImGui::Text("Dynamic");
	ImGui::Spacing();

	if (sceneModel->DynamicScenes)
	{
		if (sceneModel->DynamicScenes->size() > 0)
		{
			for (int i = 0; i < sceneModel->DynamicScenes->size(); i++)
			{
				ImGui::Text("  ");
				ImGui::SameLine();
				ImGui::Text(sceneModel->DynamicScenes->at(i)->getName().c_str());
				if (sceneModel->DynamicScenes->at(i)->getAttachedObjects().size() > 0)
				{
					for (int j = 0; j < sceneModel->DynamicScenes->at(i)->getAttachedObjects().size(); j++)
					{
						ImGui::Text("    ");
						ImGui::SameLine();
						ImGui::Text(sceneModel->DynamicScenes->at(i)->getAttachedObjects().at(j)->getName().c_str());
					}
				}

			}
		}
	}


	ImGui::Spacing();
	ImGui::Text("Static");
	ImGui::Spacing();

	if (sceneModel->StaticScenes)
	{
		if (sceneModel->StaticScenes->size() > 0)
		{
			for (int i = 0; i < sceneModel->StaticScenes->size(); i++)
			{
				ImGui::Text("  ");
				ImGui::SameLine();
				ImGui::Text(sceneModel->StaticScenes->at(i)->getName().c_str());
				if (sceneModel->StaticScenes->at(i)->getAttachedObjects().size() > 0)
				{
					for (int j = 0; j < sceneModel->StaticScenes->at(i)->getAttachedObjects().size(); j++)
					{
						ImGui::Text("    ");
						ImGui::SameLine();
						ImGui::Text(sceneModel->StaticScenes->at(i)->getAttachedObjects().at(j)->getName().c_str());
					}
				}

			}
		}
	}

	ImGui::Spacing();
	ImGui::Text("Mesh");
	ImGui::Spacing();

	if (sceneModel->MeshScenes)
	{
		if (sceneModel->MeshScenes->size() > 0)
		{
			for (int i = 0; i < sceneModel->MeshScenes->size(); i++)
			{
				ImGui::Text("  ");
				ImGui::SameLine();
				ImGui::Text(sceneModel->MeshScenes->at(i)->getName().c_str());
				if (sceneModel->MeshScenes->at(i)->getAttachedObjects().size() > 0)
				{
					for (int j = 0; j < sceneModel->MeshScenes->at(i)->getAttachedObjects().size(); j++)
					{
						ImGui::Text("    ");
						ImGui::SameLine();
						ImGui::Text(sceneModel->MeshScenes->at(i)->getAttachedObjects().at(j)->getName().c_str());
					}
				}

			}
		}
	}

	ImGui::End();

}
