#include "ObjectTabComponent.h"

void ObjectTabComponent::view()
{
	ImGui::SetNextWindowPos(ImVec2(0, 200));
	ImGui::SetNextWindowSize(ImVec2(250, 500));

	ImGui::Begin("Selected Object", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	if(objectModel->selectable){

		ImGui::Text(objectModel->selectable->selectingName->c_str());

		ImGui::Spacing();

		if (objectModel->selectable->selectedStuff) {

			ImGui::Text("Name : ");
			ImGui::SameLine();
			ImGui::Text(objectModel->selectable->selectedStuff->name.c_str());

			ImGui::Text("Position : ");
			ImGui::Text("X : ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(objectModel->selectable->selectedStuff->scnNode->getPosition().x).c_str());
			ImGui::Text("Y : ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(objectModel->selectable->selectedStuff->scnNode->getPosition().y).c_str());
			ImGui::Text("Z : ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(objectModel->selectable->selectedStuff->scnNode->getPosition().z).c_str());

			ImGui::Text("Rotation : ");
			ImGui::Text("W : ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(objectModel->selectable->selectedStuff->scnNode->getOrientation().w).c_str());
			ImGui::Text("X : ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(objectModel->selectable->selectedStuff->scnNode->getOrientation().x).c_str());
			ImGui::Text("Y : ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(objectModel->selectable->selectedStuff->scnNode->getOrientation().y).c_str());
			ImGui::Text("Z : ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(objectModel->selectable->selectedStuff->scnNode->getOrientation().z).c_str());

			if (ImGui::Button("GetFragShader"))
			{
				objectModel->getFragShader();
			}

			if (ImGui::Button("Delete"))
			{
				objectModel->deleteObject();
			}

		}
		else {
			ImGui::Text("No Object Selected!");
		}
	}
	else {
		ImGui::Text("No Selectable Set!");
	}

	

	// SHADOW MAPPING IS ENABLED SO ALL OBJECTS CAST AS WELL AS RECIEVE SHADOWS
	/*if (ImGui::Checkbox("Cast Shadow",)
	{

	}*/


	ImGui::End();


}
