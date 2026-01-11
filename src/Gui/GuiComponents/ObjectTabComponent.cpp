#include "ObjectTabComponent.h"

void ObjectTabComponent::view()
{
	ImGui::SetNextWindowSize(ImVec2(350,400));
	ImGui::SetNextWindowPos(ImVec2(0, 600));
	ImGui::Begin("Object");

	// Refresh when object is deleted and setup grid !!
	if (objectModel->selectedObj && objectModel->selectedObj->get())
	{
		ImGui::Text("Object Name : ");
		ImGui::SameLine();
		ImGui::Text(objectModel->selectedObj->get()->getName().c_str());

		

		ImGui::Spacing();

		// Mesh
		ImGui::Text("Mesh : ");
		ImGui::SameLine();
		ImGui::Text(objectModel->selectedObj->get()->getMesh()->getName().c_str());

		// Material
		if(objectModel->materials && objectModel->materials->size() > 0) {
			if (ImGui::BeginCombo("Materials", objectModel->materials->at(objectModel->selectedMaterial)->getName().c_str()))
			{
				for (int i = 0; i < objectModel->materials->size(); i++)
				{
					if (ImGui::Selectable(objectModel->materials->at(i)->getName().c_str(),objectModel->selectedMaterial == i))
					{
						objectModel->selectedMaterial = i;
					}
				}

				ImGui::EndCombo();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Set"))
		{
			objectModel->setMaterial();
		}

		ImGui::Text("Mesh Material : "); ImGui::SameLine();
		ImGui::Text(objectModel->selectedObj->get()->getMesh().get()->getSubMesh(0)->getMaterialName().c_str());

		ImGui::Text("Material : ");
		ImGui::SameLine();
		if (objectModel->selectedObj->get()->getMaterial()) {

			ImGui::Text(objectModel->selectedObj->get()->getMaterial()->getName().c_str());

			ImGui::Text("Vertex Shader : ");
			ImGui::SameLine();
			ImGui::Text(objectModel->selectedObj->get()->getMaterial()->getVertexShader()->getShaderFileName().c_str());

			ImGui::Text("Fragment Shader : ");
			ImGui::SameLine();
			ImGui::Text(objectModel->selectedObj->get()->getMaterial()->getFragmentShader()->getShaderFileName().c_str());

			if (ImGui::Button("Select Shader"))
			{
				objectModel->selectShader(objectModel->selectedObj->get()->getMaterial()->getVertexShader(),
					objectModel->selectedObj->get()->getMaterial()->getFragmentShader());
			}


		}
		else {
			ImGui::Text("No Material Selected!");
		}


	}
	else {
		ImGui::Text("No Object Selected!");
	}


	ImGui::End();


}
