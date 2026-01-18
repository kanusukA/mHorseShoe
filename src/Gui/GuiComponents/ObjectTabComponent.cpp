#include "ObjectTabComponent.h"

void ObjectTabComponent::view()
{
	ImGui::SetNextWindowSize(ImVec2(350,400));
	ImGui::SetNextWindowPos(ImVec2(0, 600));
	ImGui::Begin("Object");

	// Refresh when object is deleted and setup grid !!
	if (!ModelComponent::selectedObject->selObject.expired())
	{
		ImGui::Text("Object Name : ");
		ImGui::SameLine();
		ImGui::Text(ModelComponent::selectedObject->selObject.lock()->getName().c_str());

		

		ImGui::Spacing();

		// Mesh
		ImGui::Text("Mesh : ");
		ImGui::SameLine();
		ImGui::Text(ModelComponent::selectedObject->selObject.lock()->getMeshName().c_str());

		// Materials
		if(ModelComponent::materialDpVec && ModelComponent::materialDpVec->empty()) {
			if (ImGui::BeginCombo("Materials", ModelComponent::materialDpVec->at(objectModel->selectedMaterial).filename().string().c_str()))
			{
				for (int i = 0; i < ModelComponent::materialDpVec->size(); i++)
				{
					if (ImGui::Selectable(ModelComponent::materialDpVec->at(i).filename().string().c_str(),objectModel->selectedMaterial == i))
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
		ImGui::Text(ModelComponent::selectedObject->selObject.lock()->getMeshMaterialName().c_str());

		ImGui::Text("Material : ");
		ImGui::SameLine();
		if (!ModelComponent::selectedMaterial->selMaterial.expired()) {

			ImGui::Text(ModelComponent::selectedMaterial->selMaterial.lock()->getName().c_str());

			ImGui::Text("Vertex Shader : ");
			ImGui::SameLine();
			ImGui::Text(ModelComponent::selectedMaterial->selMaterial.lock()->getVertexShaderName().c_str());

			ImGui::Text("Fragment Shader : ");
			ImGui::SameLine();
			ImGui::Text(ModelComponent::selectedMaterial->selMaterial.lock()->getFragmentShaderName().c_str());

			if (ImGui::Button("Select Shader"))
			{
				ModelComponent::selectedMaterial->selMaterial.lock()->selectShader(); // TODO DOES NOT WORK YET!!!!
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
